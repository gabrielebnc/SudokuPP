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
    int startingTemp;

    void reset() {
        currentBoard = startingBoard;
    }

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

    /*
     * Standard deviation of a number of randomly initialized states
     * */
    int starting_temperature(int n_states = 200) {
        std::cout << "Computing starting temp \n";
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
            stdev += std::pow(state-mean, 2);
        }
        stdev /= n_states;
        reset();
        return int(std::sqrt(stdev));
    }

    std::bitset<SUDOKU_SIZE> seen_elements_cell(int startRow, int startCol) {
        std::bitset<SUDOKU_SIZE> seen;
        for (int row = startRow; row < startRow + SUDOKU_CELL_SIZE; ++row) {
            for (int col = startCol; col < startCol + SUDOKU_CELL_SIZE; ++col) {
                // It is guaranteed that we will never set the same bit twice
                int elem = currentBoard[row][col];
                if (elem != 0)
                    seen.set(currentBoard[row][col] - 1);
            }
        }
        return seen;
    }

    void random_fill_cell(int startRow, int startCol) {
        std::bitset<SUDOKU_SIZE> seen = seen_elements_cell(startRow, startCol);
        for (int row = startRow; row < startRow + SUDOKU_CELL_SIZE; ++row) {
            for (int col = startCol; col < startCol + SUDOKU_CELL_SIZE; ++col) {
                int &elem = currentBoard[row][col];
                if (startingBoard[row][col] == 0){
                    while (true) {
                        int candidate = randomIntInRange(1, SUDOKU_SIZE);
                        if(!seen[candidate-1]){
                            elem = candidate;
                            seen.set(candidate - 1);
                            break;
                        }
                    }
                }
                seen.reset();
            }
        }
    }

    void random_fill_sudoku() {
        for (int startRow = 0; startRow < SUDOKU_SIZE; startRow += SUDOKU_CELL_SIZE) {
            for (int startCol = 0; startCol < SUDOKU_SIZE; startCol += SUDOKU_CELL_SIZE) {
                random_fill_cell(startRow, startCol);
            }
        }
        display_current_board();
    }

public:

    explicit Sudoku(const std::vector<std::vector<int>> &board, int n_samples_iT) : startingBoard(board),
                                                                                    currentBoard(board) {
        startingTemp = starting_temperature(n_samples_iT);
    };

    /*
     * Simulated Annealing solving
     *
     * @param iT Initial Temperature for the Simulated Annealing
     * @param alpha Cooling Factor for the Simulated Annealing
     * @param iterations Maximum iterations for the Simulated Annealing
     * @return True if operation was successful, reaching objective status 0
     * */
    bool solve(int iT, int alpha, int iterations, int n_states_iT = 200) {

    }

    void display_starting_board() {
        display_board(startingBoard);
        std::cout << "Starting Temp: " << startingTemp << "\n";
    };

    void display_current_board() {
        display_board(currentBoard);
        std::cout << "Violation count: " << objective_status() << std::endl;
    };

    void display_random_filled_board() {
        random_fill_sudoku();
        display_current_board();
        reset();
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

    auto sudoku = Sudoku(s, 5);
    sudoku.display_starting_board();
}
