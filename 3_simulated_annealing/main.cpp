#include <iostream>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <random>
#include <cmath>
#include <chrono>

const int SUDOKU_SIZE = 9;
const int SUDOKU_CELL_SIZE = 3;

static std::random_device rd;
static std::mt19937 gen(rd());

int randomIntInRange(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

double randomDoubleInRange(double min, double max) {
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

class Sudoku {
private:
    std::vector<std::vector<int>> startingBoard;
    std::vector<std::vector<int>> currentBoard;


    [[maybe_unused]] bool setCell(int row, int col, int val) {
        if (startingBoard[row][col] != 0) return false;
        currentBoard[row][col] = val;
        return true;
    }

    static void display_board(const std::vector<std::vector<int>> &board) {
        for (const auto &row: board) {
            for (int i = 0; i < SUDOKU_SIZE; ++i) {
                if (i % 3 == 0) std::cout << "|";
                std::cout << " " << row[i] << " ";
            }
            std::cout << "|\n";
        }
    }

    static int row_violations(const std::vector<std::vector<int>> &sudoku) {
        int violations = 0;
        std::unordered_map<int, int> counts;
        for (const auto &row: sudoku) {
            for (auto num: row) {
                counts[num]++;
            }
            for (const auto &entry: counts) {
                if (entry.second > 1) {
                    violations += (entry.second - 1);
                }
            }
            counts.clear();
        }
        return violations;
    }

    static int col_violations(const std::vector<std::vector<int>> &sudoku) {
        int violations = 0;
        std::unordered_map<int, int> counts;
        for (int row = 0; row < SUDOKU_SIZE; ++row) {
            for (int col = 0; col < SUDOKU_SIZE; ++col) {
                counts[sudoku[row][col]]++;
            }
            for (const auto &entry: counts) {
                if (entry.second > 1) {
                    violations += (entry.second - 1);
                }
            }
            counts.clear();
        }
        return violations;
    }

    static int cell_violations(const std::vector<std::vector<int>> &sudoku) {
        int violations = 0;
        std::unordered_map<int, int> counts;
        for (int startRow = 0; startRow < SUDOKU_SIZE; startRow += SUDOKU_CELL_SIZE) {
            for (int startCol = 0; startCol < SUDOKU_SIZE; startCol += SUDOKU_CELL_SIZE) {
                for (int row = startRow; row < startRow + SUDOKU_CELL_SIZE; ++row) {
                    for (int col = startCol; col < startCol + SUDOKU_CELL_SIZE; ++col) {
                        counts[sudoku[row][col]]++;

                    }
                }

                for (const auto &entry: counts) {
                    if (entry.second > 1) {
                        violations += (entry.second - 1);
                    }
                }
                counts.clear();
            }
        }
        return violations;
    }

    // Evaluation of current board status, if 0 the board is solved
    static int sudoku_evaluation(const std::vector<std::vector<int>> &sudoku) {
        return row_violations(sudoku) + col_violations(sudoku) + cell_violations(sudoku);
    }


    std::bitset<SUDOKU_SIZE> seen_elements_cell(int startRow, int startCol) {
        std::bitset<SUDOKU_SIZE> seen;
        for (int row = startRow; row < startRow + SUDOKU_CELL_SIZE; ++row) {
            for (int col = startCol; col < startCol + SUDOKU_CELL_SIZE; ++col) {
                // It is guaranteed that we will never set the same bit twice
                int elem = startingBoard[row][col];
                if (elem != 0)
                    seen.set(startingBoard[row][col] - 1);
            }
        }
        return seen;
    }

    void random_fill_cell(int startRow, int startCol) {
        std::bitset<SUDOKU_SIZE> seen = seen_elements_cell(startRow, startCol);
        for (int row = startRow; row < startRow + SUDOKU_CELL_SIZE; ++row) {
            for (int col = startCol; col < startCol + SUDOKU_CELL_SIZE; ++col) {
                int &elem = currentBoard[row][col];
                if (startingBoard[row][col] == 0) {
                    while (true) {
                        int candidate = randomIntInRange(1, SUDOKU_SIZE);
                        if (!seen[candidate - 1]) {
                            elem = candidate;
                            seen.set(candidate - 1);
                            break;
                        }
                    }
                }
            }
        }
    }

    void random_fill_sudoku() {
        for (int startRow = 0; startRow < SUDOKU_SIZE; startRow += SUDOKU_CELL_SIZE) {
            for (int startCol = 0; startCol < SUDOKU_SIZE; startCol += SUDOKU_CELL_SIZE) {
                random_fill_cell(startRow, startCol);
            }
        }
    }

    static std::pair<int, int> pick_random_cell() {
        return {randomIntInRange(0, 2) * SUDOKU_CELL_SIZE, randomIntInRange(0, 2) * SUDOKU_CELL_SIZE};
    }

    void swap_pair_in_cell(const std::pair<int, int> &cell) {

        // Get random indices for first cell
        int row1 = randomIntInRange(cell.first, cell.first + 2);
        int col1 = randomIntInRange(cell.second, cell.second + 2);
        int &first = currentBoard[row1][col1]; // Reference to the first cell value

        int row2, col2;

        // Ensure second cell is different from the first
        do {
            row2 = randomIntInRange(cell.first, cell.first + 2);
            col2 = randomIntInRange(cell.second, cell.second + 2);
        } while ((row1 == row2 && col1 == col2));

        int &second = currentBoard[row2][col2];

        int temp = second;
        second = first;
        first = temp;
    }

    static bool acceptance_function(double T, double delta_eval) {
        if (randomDoubleInRange(0, 1) < std::exp(-delta_eval / T)) return true;
        return false;
    }


public:
    explicit Sudoku(const std::vector<std::vector<int>> &board) : startingBoard(board),
                                                                  currentBoard(board) {
    };

    void reset() {
        currentBoard = startingBoard;
    }

    int current_sudoku_evaluation() {
        return sudoku_evaluation(currentBoard);
    }

    /*
     * Standard deviation of a number of randomly initialized states
     * */
    int starting_temperature(int n_states = 200) {
        double mean = 0;
        double stdev = 0;
        std::vector<double> states{};

        for (int i = 0; i < n_states; ++i) {
            random_fill_sudoku();
            double status = current_sudoku_evaluation();
            states.push_back(status);
            mean += double(status) / n_states;
        }

        for (double state: states) {
            stdev += std::pow(state - mean, 2);
        }
        stdev /= n_states;
        reset();
        return int(std::sqrt(stdev));
    }

    void compute_neighbor_position() {
        auto cell = pick_random_cell();
        swap_pair_in_cell(cell);
    }

    /*
     * Simulated Annealing solving
     *
     * @param iT Initial Temperature for the Simulated Annealing
     * @param alpha Cooling Factor for the Simulated Annealing
     * @param iterations Maximum iterations for the Simulated Annealing
     * @return True if operation was successful, reaching objective status 0
     * */
    bool solve(double iT, double alpha, int iterations) {
        double T = iT;
        bool solution_found = false;
        random_fill_sudoku();
        int current_eval = current_sudoku_evaluation();
        std::vector<std::vector<int>> best_board = currentBoard;
        std::vector<std::vector<int>> current_board = currentBoard;

        for (int i = 0; i < iterations; ++i) {

            if (current_eval == 0) {
                solution_found = true;
                break;
            }

            compute_neighbor_position();
            int new_eval = current_sudoku_evaluation();
            int delta_eval = new_eval - current_eval;

            if (delta_eval < 0) {
                best_board = currentBoard;
                current_eval = new_eval;
                current_board = currentBoard;
            }
            if (acceptance_function(T, delta_eval)) {
                current_eval = new_eval;
                current_board = currentBoard;
            } else { // the board got into a worse status and also didn't pass the acceptance function, revert it to current_board
                currentBoard = current_board;
            }

            T *= alpha;
        }

        currentBoard = best_board;
        return solution_found;
    }

    [[maybe_unused]] void display_starting_board() {
        std::cout << "Starting board:\n";
        display_board(startingBoard);
    };

    [[maybe_unused]] void display_current_board() {
        std::cout << "Current board:\n";
        display_board(currentBoard);
        std::cout << "Violation count: " << current_sudoku_evaluation() << std::endl;
    };

    [[maybe_unused]] void display_random_filled_board() {
        random_fill_sudoku();
        display_current_board();
    }

};


int main() {

    std::vector<std::vector<int>> s = {
            {0, 0, 0, 0, 0, 0, 9, 0, 1},
            {0, 1, 0, 0, 7, 0, 8, 0, 0},
            {0, 0, 8, 0, 6, 0, 0, 2, 0},

            {0, 0, 2, 0, 5, 0, 0, 3, 0},
            {4, 0, 0, 7, 0, 0, 0, 8, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},

            {0, 0, 1, 9, 0, 0, 0, 0, 0},
            {0, 2, 0, 4, 0, 0, 0, 9, 3},
            {0, 0, 5, 3, 0, 0, 0, 7, 0}
    };

    std::cout << std::boolalpha;

    auto sudoku = Sudoku(s);
    int initial_temp = sudoku.starting_temperature(30);
    double alpha = 0.85;
    int max_iter = 3000;
    int benchmark_iters = 100;
    int correct_count = 0;

    long long average = 0;
    for (int i = 0; i < benchmark_iters; ++i) {
        sudoku.reset();
        auto start = std::chrono::high_resolution_clock::now();
        sudoku.solve(initial_temp, alpha, max_iter);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        average += duration.count();
        correct_count += (sudoku.current_sudoku_evaluation() == 0);
    }
    average /= benchmark_iters;
    sudoku.display_current_board();

    std::cout << "Average execution time: " << average << "ms = " << float(average) / 1000 << "s"
              << " with alpha = " << alpha << " and loops = " << max_iter << "\n";
    std::cout << "Correct solutions in benchmark: " << correct_count << "/" << benchmark_iters << std::endl;
}
