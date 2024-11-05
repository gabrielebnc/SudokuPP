#include <iostream>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <random>
#include <cmath>

const int SUDOKU_SIZE = 9;
const int SUDOKU_CELL_SIZE = 3;

static std::random_device rd;
static std::mt19937 gen(rd());

int randomIntInRange(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
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

    int row_violations() {
        int violations = 0;
        std::unordered_map<int, int> counts;
        for (const auto &row: currentBoard) {
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

    int col_violations() {
        int violations = 0;
        std::unordered_map<int, int> counts;
        for (int row = 0; row < SUDOKU_SIZE; ++row) {
            for (int col = 0; col < SUDOKU_SIZE; ++col) {
                counts[currentBoard[row][col]]++;
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

    int cell_violations() {
        int violations = 0;
        std::unordered_map<int, int> counts;
        for (int startRow = 0; startRow < SUDOKU_SIZE; startRow += SUDOKU_CELL_SIZE) {
            for (int startCol = 0; startCol < SUDOKU_SIZE; startCol += SUDOKU_CELL_SIZE) {
                for (int row = startRow; row < startRow + SUDOKU_CELL_SIZE; ++row) {
                    for (int col = startCol; col < startCol + SUDOKU_CELL_SIZE; ++col) {
                        counts[currentBoard[row][col]]++;

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
    int objective_status() {
        return row_violations() + col_violations() + cell_violations();
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

    std::pair<int, int> pick_random_cell() {
        return {randomIntInRange(0, 2) * SUDOKU_CELL_SIZE, randomIntInRange(0, 2) * SUDOKU_CELL_SIZE};
    }

    int &random_cell_elem(const std::pair<int, int> &cell) {
        std::cout << "CELL: " << cell.first << "," << cell.second << "\n";
        int row = randomIntInRange(cell.first, cell.first + 2);
        int col = randomIntInRange(cell.second, cell.second + 2);
        std::cout << "ELEM: " << row << "," << col << "\n";
        return currentBoard[row][col];
    }

    void swap_pair_in_cell(const std::pair<int, int> &cell) {
        int &first = random_cell_elem(cell);
        int &second = random_cell_elem(cell);
        while (first == second) {
            second = random_cell_elem(cell);
        }
        int temp = second;
        second = first;
        first = temp;
    }


public:
    explicit Sudoku(const std::vector<std::vector<int>> &board) : startingBoard(board),
                                                                  currentBoard(board) {
    };

    void reset() {
        currentBoard = startingBoard;
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
            double status = objective_status();
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
    bool solve(int iT, double alpha, int iterations) {
        random_fill_sudoku(); // Get an initial solution to use as start
        auto &start = currentBoard;
        int curr_eval = objective_status();
        int best_eval = curr_eval;

        auto &neighbor = start;
        int iter = 0;
        while (iter < iterations) {
            compute_neighbor_position();
            neighbor = currentBoard;
            curr_eval = objective_status();

            ++iter;
        }
    }

    void display_starting_board() {
        std::cout << "Starting board:\n";
        display_board(startingBoard);
    };

    void display_current_board() {
        std::cout << "Current board:\n";
        display_board(currentBoard);
        std::cout << "Violation count: " << objective_status() << std::endl;
    };

    void display_random_filled_board() {
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

    auto sudoku = Sudoku(s);
    sudoku.display_starting_board();
    std::cout << "STARTING TEMP: " << sudoku.starting_temperature(10) << "\n";
    sudoku.display_random_filled_board();
    sudoku.compute_neighbor_position();
    sudoku.display_current_board();

}
