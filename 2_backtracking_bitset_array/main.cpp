#include <iostream>
#include <bitset>
#include <chrono>

const int SUDOKU_SIZE = 9;
const int SUDOKU_CELL_SIZE = 3;

/*
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

/*
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

/*
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

/*
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

/*
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

/*
 * Detects violation in sudoku's composition
 * */
bool is_sudoku_violated(const int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]) {
    return is_row_violated(sudoku) || is_col_violated(sudoku) || is_cells_violated(sudoku);
}

/*
 * Sudoku solver with backtracking
 * Modifies the sudoku in-place
 * Returns wether the operation is successful or not
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

int main() {
    int sudoku[SUDOKU_SIZE][SUDOKU_SIZE] = {
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

    auto start = std::chrono::high_resolution_clock::now();
    auto result = solve_sudoku(sudoku);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << std::boolalpha;
    std::cout << "Solve op result: " << result << "\n";

    display_sudoku(sudoku);
    std::cout << "Execution time: " << duration.count() << "ms = " << float(duration.count()) / 1000 << "s" << std::endl;
}
