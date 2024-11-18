#ifndef SUDOKUPP_BACKTRACKING_NAIVE_H
#define SUDOKUPP_BACKTRACKING_NAIVE_H

#include <vector>

bool solve_sudoku(std::vector<std::vector<int>> &sudoku);

[[maybe_unused]]void display_sudoku(const std::vector<std::vector<int>> &sudoku);

#endif //SUDOKUPP_BACKTRACKING_NAIVE_H
