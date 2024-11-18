#include "backtracking_naive.h"

#include <iostream>
#include <vector>
#include <set>

const int SUDOKU_SIZE = 9;
const int SUDOKU_CELL_SIZE = 3;

/**
 * Utility function to display the sudoku
 * */
[[maybe_unused]] void display_sudoku(const std::vector<std::vector<int>> &sudoku) {
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            if (j % 3 == 0) std::cout << "|";
            std::cout << " " << sudoku[i][j] << " ";
        }
        std::cout << "|\n";
    }
}

/**
 * Detects violation in sudoku's rows
 * */
bool is_row_violated(const std::vector<std::vector<int>> &sudoku) {
    for (const auto &row: sudoku) {
        std::set<int> seen;
        for (auto num: row) {
            if (num == 0) continue;
            if (seen.find(num) != seen.end()) {
                return true;
            }
            seen.insert(num);
        }
    }
    return false;
}

/**
 * Detects violation in sudoku's columns
 * */
bool is_col_violated(const std::vector<std::vector<int>> &sudoku) {
    for (int i = 0; i < SUDOKU_SIZE; ++i) {
        std::set<int> seen;
        for (int j = 0; j < SUDOKU_SIZE; ++j) {
            int elem = sudoku[j][i];
            if (elem == 0) continue;
            if (seen.find(elem) != seen.end()) {
                return true;
            }
            seen.insert(elem);
        }
    }
    return false;
}

/**
 * Detects violation in sudoku's given cell
 * */
bool is_cell_violated(const std::vector<std::vector<int>> &sudoku, int startRow, int startCol) {
    std::set<int> seen;
    for (int row = startRow; row < startRow + SUDOKU_CELL_SIZE; ++row) {
        for (int col = startCol; col < startCol + SUDOKU_CELL_SIZE; ++col) {
            int elem = sudoku[row][col];
            if (elem == 0) continue;
            if (seen.find(elem) != seen.end()) {
                return true;
            }
            seen.insert(elem);
        }
    }
    return false;
}

/**
 * Detects violation in sudoku's cells
 * */
bool is_cells_violated(const std::vector<std::vector<int>> &sudoku) {
    for (int row = 0; row < SUDOKU_SIZE; row += SUDOKU_CELL_SIZE) {
        for (int col = 0; col < SUDOKU_SIZE; col += SUDOKU_CELL_SIZE) {
            if (is_cell_violated(sudoku, row, col)) return true;
        }
    }
    return false;
}

/**
 * Detects violation in sudoku's composition
 * */
bool is_sudoku_violated(const std::vector<std::vector<int>> &sudoku) {
    return is_row_violated(sudoku) || is_col_violated(sudoku) || is_cells_violated(sudoku);
}

/**
 * Sudoku solver with backtracking
 * Modifies the sudoku in-place
 * @return wether the operation is successful or not
 * */
bool solve_sudoku(std::vector<std::vector<int>> &sudoku) {

    if (is_sudoku_violated(sudoku)) {
        return false;
    }
    for (auto &row: sudoku) {
        for (int &elem: row) {
            if (elem == 0) {
                for (int insertNum = 1; insertNum <= SUDOKU_SIZE; ++insertNum) {
                    // we always set the element
                    elem = insertNum;
                    if (solve_sudoku(sudoku)) {
                        return true;
                    }
                    // if setting the element violates the sudoku rules, it gets "deleted" to try a new element
                    elem = 0;
                }
                return false;
            }
        }
    }
    return true;
}
