#include <iostream>
#include "SABenchmark.h"
#include "SASolver.h"

SABenchmark::SABenchmark() = default;

std::vector<std::vector<int>> SABenchmark::string_to_sudoku(const std::string &sudoku_string) {
    std::vector<std::vector<int>> sudoku(9, std::vector<int>(9));
    for (auto i = 0; i < sudoku_string.length(); ++i) {
        int row = i / SUDOKU_SIZE;
        int col = i % SUDOKU_SIZE;
        sudoku[row][col] = handle_char(sudoku_string[i]);
    }
    return sudoku;
}

void SABenchmark::run_benchmark() {
    int count = 1;
    auto test_len = getSudokuTestsAsString().size();
    for (const auto &sudoku_string: getSudokuTestsAsString()) {
        std::cout << "Running test " << count++ << "/" << test_len << std::endl;
        auto sas = SASolver(string_to_sudoku(sudoku_string));
        auto initial_temp = sas.starting_temperature();
        auto start = std::chrono::high_resolution_clock::now();
        sas.solve(initial_temp, SA_SOLVER_ALPHA, SA_SOLVER_MAX_ITER);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        sas.display_current_board();
        addMsDuration(duration.count());
    }
    print_benchmark_results("BTBitset");
}
