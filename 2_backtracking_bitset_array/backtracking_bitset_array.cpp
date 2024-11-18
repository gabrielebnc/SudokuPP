#include "backtracking_bitset_array.h"
#include <iostream>
#include <bitset>

const int SUDOKU_CELL_SIZE = 3;

/**
 * Utility function to display the sudoku
 * */
void display_sudoku(const int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]) {
    for (const auto &row: sudoku) {
        for (int i = 0; i < SUDOKU_SIZE; ++i) {
            if (i % 3 == 0) std::cout << "|";
            std::cout << " " << row[i] << " ";
        }
        std::cout << "|\n";
    }
}

/**
 * Detects violation in sudoku's rows
 * */
bool is_row_violated(const int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]) {
    std::bitset<SUDOKU_SIZE> seen;
    for (const auto &row: sudoku) {
        for (auto elem: row) {
            if (elem == 0) continue;
            if (seen[elem - 1]) {
                return true;
            }
            seen.set(elem - 1);
        }
        seen.reset();
    }
    return false;
}

/**
 * Detects violation in sudoku's columns
 * */
bool is_col_violated(const int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]) {
    std::bitset<SUDOKU_SIZE> seen;
    for (int i = 0; i < SUDOKU_SIZE; ++i) {
        for (int j = 0; j < SUDOKU_SIZE; ++j) {
            int elem = sudoku[j][i];
            if (elem == 0) continue;
            if (seen[elem - 1]) {
                return true;
            }
            seen.set(elem - 1);
        }
        seen.reset();
    }
    return false;
}

/**
 * Detects violation in sudoku's given cell
 * */
bool is_cell_violated(const int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE], int startRow, int startCol) {
    std::bitset<SUDOKU_SIZE> seen;
    for (int row = startRow; row < startRow + SUDOKU_CELL_SIZE; ++row) {
        for (int col = startCol; col < startCol + SUDOKU_CELL_SIZE; ++col) {
            int elem = sudoku[row][col];
            if (elem == 0) continue;
            if (seen[elem - 1]) {
                return true;
            }
            seen.set(elem - 1);
        }
    }
    return false;
}

/**
 * Detects violation in sudoku's cells
 * */
bool is_cells_violated(const int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]) {
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
bool is_sudoku_violated(const int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]) {
    return is_row_violated(sudoku) || is_col_violated(sudoku) || is_cells_violated(sudoku);
}

/**
 * Sudoku solver with backtracking
 * Modifies the sudoku in-place
 * @return wether the operation is successful or not
 * */
bool solve_sudoku(int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]) {

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
