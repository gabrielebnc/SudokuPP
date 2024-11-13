#include "BenchmarkAbstract.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

const int SUDOKU_BOARD_LENGTH = 81; // 9x9 Sudokus

BenchmarkAbstract::BenchmarkAbstract(std::filesystem::path sudoku_tests_path) : path_to_tests(
        std::move(sudoku_tests_path)) {

    read_sudoku_test_file(path_to_tests); // Call to read Sudoku test file
}


void BenchmarkAbstract::read_sudoku_test_file(const std::filesystem::path &sudoku_tests_path) {


    std::ifstream sudoku_file(sudoku_tests_path);
    if (!sudoku_file) {
        std::cerr << "Error opening Sudoku tests for benchmarking. File: " << sudoku_tests_path << std::endl;
        throw std::runtime_error("Failed to open file: " + sudoku_tests_path.string());
    }
    std::vector<std::string> sudoku_strings{};
    std::string line;
    while (std::getline(sudoku_file, line)) {
        if (line.length() == SUDOKU_BOARD_LENGTH) {
            sudoku_strings.push_back(line);
        } else {
            std::cout << "Invalid Sudoku (reason: length): " << line << std::endl;
        }
    }
    sudoku_file.close();
    sudoku_tests_as_string = sudoku_strings;
}


std::vector<std::string> BenchmarkAbstract::get_sudoku_tests_as_string() const {
    return sudoku_tests_as_string;
}

int BenchmarkAbstract::handle_char(char c) {
    int num = c - '0';
    if (num < 0 || num > 9) return 0;
    return num;
}
