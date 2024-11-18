#include <iostream>
#include "NaiveBenchmark.h"
#include "backtracking_naive.h"

constexpr int SUDOKU_SIZE = 9;

NaiveBenchmark::NaiveBenchmark() = default;

std::vector<std::vector<int>> NaiveBenchmark::string_to_sudoku(const std::string &sudoku_string) {
    std::vector<std::vector<int>> sudoku(9, std::vector<int>(9));
    for (auto i = 0; i < sudoku_string.length(); ++i) {
        int row = i / SUDOKU_SIZE;
        int col = i % SUDOKU_SIZE;
        sudoku[row][col] = handle_char(sudoku_string[i]);
    }
    return sudoku;
}

void NaiveBenchmark::run_benchmark() {
    int count = 1;
    auto test_len = getSudokuTestsAsString().size();
    for (const auto &sudoku_string: getSudokuTestsAsString()) {
        std::cout << "Running test " << count++ << "/" << test_len << std::endl;
        auto sudoku = string_to_sudoku(sudoku_string);
        auto start = std::chrono::high_resolution_clock::now();
        solve_sudoku(sudoku);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        addMsDuration(duration.count());
    }
    print_benchmark_results("Naive");
}

