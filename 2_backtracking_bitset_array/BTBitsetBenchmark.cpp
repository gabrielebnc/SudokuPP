#include "BTBitsetBenchmark.h"
#include "backtracking_bitset_array.h"
#include <iostream>

BTBitsetBenchmark::BTBitsetBenchmark() = default;


int (&BTBitsetBenchmark::string_to_sudoku(const std::string &sudoku_string,
                                          int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]))[SUDOKU_SIZE][SUDOKU_SIZE] {


    for (auto i = 0; i < sudoku_string.length(); ++i) {
        int row = i / SUDOKU_SIZE;
        int col = i % SUDOKU_SIZE;
        sudoku[row][col] = handle_char(sudoku_string[i]);
    }

    return sudoku;
}

void BTBitsetBenchmark::run_benchmark() {

    int count = 1;
    auto test_len = getSudokuTestsAsString().size();
    int sudoku[SUDOKU_SIZE][SUDOKU_SIZE];
    for (const auto &sudoku_string: getSudokuTestsAsString()) {

        std::cout << "Running test " << count++ << "/" << test_len << std::endl;
        //Modifies sudoku in place
        string_to_sudoku(sudoku_string, sudoku);
        auto start = std::chrono::high_resolution_clock::now();
        solve_sudoku(sudoku);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        display_sudoku(sudoku);
        addMsDuration(duration.count());
    }
    print_benchmark_results("BTBitset");

}


