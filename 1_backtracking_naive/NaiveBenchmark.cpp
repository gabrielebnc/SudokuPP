#include "NaiveBenchmark.h"
#include "backtracking_naive.h"

const int SUDOKU_SIZE = 9;

NaiveBenchmark::NaiveBenchmark(const std::filesystem::path &sudoku_tests_path)
        : BenchmarkAbstract(sudoku_tests_path) {
}

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
    for (const auto &sudoku_string: getSudokuTestsAsString()) {
        auto sudoku = string_to_sudoku(sudoku_string);
        auto start = std::chrono::high_resolution_clock::now();
        solve_sudoku(sudoku);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        addMsDuration(duration.count()
        );
    }
    print_benchmark_results("Naive");
}