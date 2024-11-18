#ifndef SUDOKUPP_BACKTRACKING_BITSET_ARRAY_H
#define SUDOKUPP_BACKTRACKING_BITSET_ARRAY_H

const int SUDOKU_SIZE = 9;

bool solve_sudoku(int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]);

void display_sudoku(const int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]);

#endif //SUDOKUPP_BACKTRACKING_BITSET_ARRAY_H
