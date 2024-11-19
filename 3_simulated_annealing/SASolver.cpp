#include "SASolver.h"
#include <iostream>
#include <unordered_map>
#include <bitset>
#include <vector>
#include <cmath>
#include <utils/math.h>

const int SUDOKU_CELL_SIZE = 3;

[[maybe_unused]] bool SASolver::setCell(int row, int col, int val) {
    if (startingBoard[row][col] != 0) return false;
    currentBoard[row][col] = val;
    return true;
}

void SASolver::display_board(const std::vector<std::vector<int>> &board) {
    for (const auto &row: board) {
        for (int i = 0; i < SUDOKU_SIZE; ++i) {
            if (i % 3 == 0) std::cout << "|";
            std::cout << " " << row[i] << " ";
        }
        std::cout << "|\n";
    }
}

int SASolver::row_violations(const std::vector<std::vector<int>> &sudoku) {
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

// FIXME gives wrong results!
int SASolver::col_violations(const std::vector<std::vector<int>> &sudoku) {
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

// Not used: the evaluation function can use just the row violations
int SASolver::cell_violations(const std::vector<std::vector<int>> &sudoku) {
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

/**
 * Sudoku Evaluation
 * */
int SASolver::sudoku_evaluation(const std::vector<std::vector<int>> &sudoku) {
    // TODO faster solution???
    return row_violations(sudoku) + col_violations(sudoku);
}


std::bitset<SUDOKU_SIZE> SASolver::seen_elements_cell(int startRow, int startCol) {
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

void SASolver::random_fill_cell(int startRow, int startCol) {
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

void SASolver::random_fill_sudoku() {
    for (int startRow = 0; startRow < SUDOKU_SIZE; startRow += SUDOKU_CELL_SIZE) {
        for (int startCol = 0; startCol < SUDOKU_SIZE; startCol += SUDOKU_CELL_SIZE) {
            random_fill_cell(startRow, startCol);
        }
    }
}

std::pair<int, int> SASolver::pick_random_cell() {
    return {randomIntInRange(0, 2) * SUDOKU_CELL_SIZE, randomIntInRange(0, 2) * SUDOKU_CELL_SIZE};
}

void SASolver::swap_pair_in_cell(const std::pair<int, int> &cell) {

    // Get random indices for first cell
    int row1, col1;

    do {
        row1 = randomIntInRange(cell.first, cell.first + 2);
        col1 = randomIntInRange(cell.second, cell.second + 2);
    } while (startingBoard[row1][col1] != 0);

    int &first = currentBoard[row1][col1]; // Reference to the first cell value

    int row2, col2;

    // Ensure second cell is different from the first
    do {
        row2 = randomIntInRange(cell.first, cell.first + 2);
        col2 = randomIntInRange(cell.second, cell.second + 2);
    } while ((row1 == row2 && col1 == col2) || startingBoard[row2][col2] != 0);

    int &second = currentBoard[row2][col2];

    int temp = second;
    second = first;
    first = temp;
}

bool SASolver::acceptance_function(double T, double delta_eval) {
    if (randomDoubleInRange(0, 1) < std::exp(-delta_eval / T)) return true;
    return false;
}


SASolver::SASolver(const std::vector<std::vector<int>> &board) : startingBoard(board),
                                                              currentBoard(board){}

void SASolver::reset() {
    currentBoard = startingBoard;
}

int SASolver::current_sudoku_evaluation() {
    return sudoku_evaluation(currentBoard);
}

/**
 * Standard deviation of a number of randomly initialized states
 * */
int SASolver::starting_temperature(int n_states){
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

void SASolver::compute_neighbor_position() {
    auto cell = pick_random_cell();
    swap_pair_in_cell(cell);
}

/**
 * Simulated Annealing solving
 *
 * @param iT Initial Temperature for the Simulated Annealing
 * @param alpha Cooling Factor for the Simulated Annealing
 * @param iterations Maximum iterations for the Simulated Annealing
 * @return True if operation was successful, reaching objective status 0
 * */
bool SASolver::solve(double iT, double alpha, int iterations) {
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

[[maybe_unused]] void SASolver::display_starting_board() {
    std::cout << "Starting board:\n";
    display_board(startingBoard);
}

[[maybe_unused]] void SASolver::display_current_board() {
    std::cout << "Current board:\n";
    display_board(currentBoard);
    std::cout << "Violation count: " << current_sudoku_evaluation() << std::endl;
}

[[maybe_unused]] void SASolver::display_random_filled_board() {
    random_fill_sudoku();
    display_current_board();
}