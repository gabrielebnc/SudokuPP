#include "BenchmarkAbstract.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <utils/math.h>

const std::filesystem::path LOCAL_FILE_DIRECTORY = __FILE__;
const std::string SUDOKU_TESTS_FILENAME = "sudoku_benchmark_test.txt";
const std::filesystem::path BENCHMARK_TESTS_PATH = LOCAL_FILE_DIRECTORY.parent_path() / SUDOKU_TESTS_FILENAME;

constexpr int SUDOKU_BOARD_LENGTH = 81; // 9x9 Sudokus

BenchmarkAbstract::BenchmarkAbstract() : path_to_tests(BENCHMARK_TESTS_PATH), ms_durations(), benchmark_size() {
    read_sudoku_test_file(path_to_tests); // Call to read Sudoku test file
}

void BenchmarkAbstract::read_sudoku_test_file(const std::filesystem::path &sudoku_tests_path) {

    std::ifstream sudoku_file(sudoku_tests_path);
    if (!sudoku_file) {
        std::cerr << "Error opening Sudoku tests for benchmarking. File: " << sudoku_tests_path << std::endl;
        throw std::runtime_error("Failed to open file: " + sudoku_tests_path.string());
    }
    std::string line;
    while (std::getline(sudoku_file, line)) {
        if (line.length() == SUDOKU_BOARD_LENGTH) {
            addSudokuString(line);
        } else {
            std::cout << "Invalid Sudoku (reason: length): " << line << std::endl;
        }
    }
    sudoku_file.close();
    setBenchmarkSize(sudoku_tests_as_string.size());
}

int BenchmarkAbstract::handle_char(char c) {
    int num = c - '0';
    if (num < 0 || num > 9) return 0;
    return num;
}

void BenchmarkAbstract::print_benchmark_results(const std::optional<std::string> &bench_name,
                                                int incorrect_results_count) const {
    long double _mean = mean(getMsDurations());
    int bench_size = static_cast<int>(getBenchmarkSize());

    std::cout << bench_name.value_or("Benchmark") << " results:\n";
    std::cout << "Executed Benchmark on " << getBenchmarkSize() << " testing puzzles.\n";

    if (incorrect_results_count != 0) {
        std::cout << incorrect_results_count << "/" << getBenchmarkSize() << " ("
                  << std::fixed << std::setprecision(2) << percentage(incorrect_results_count, bench_size)
                  << "%) INCORRECT RESULTS!\n";
    } else {
        std::cout << "All results are correct.\n";
    }

    std::cout << "Average execution: " << _mean << "ms = " << _mean / 1000 << "s.\n";
    std::cout << "Standard deviation: " << standardDeviation(getMsDurations()) << std::endl;
}

const std::vector<std::string> &BenchmarkAbstract::getSudokuTestsAsString() const {
    return sudoku_tests_as_string;
}

[[maybe_unused]] void BenchmarkAbstract::setSudokuTestsAsString(const std::vector<std::string> &sudokuTestsAsString) {
    sudoku_tests_as_string = sudokuTestsAsString;
}

size_t BenchmarkAbstract::getBenchmarkSize() const {
    return benchmark_size;
}

void BenchmarkAbstract::setBenchmarkSize(size_t benchmarkSize) {
    benchmark_size = benchmarkSize;
}

const std::vector<long long int> &BenchmarkAbstract::getMsDurations() const {
    return ms_durations;
}

[[maybe_unused]] void BenchmarkAbstract::setMsDurations(const std::vector<long long int> &msDurations) {
    ms_durations = msDurations;
}

void BenchmarkAbstract::addMsDuration(long long msDuration) {
    ms_durations.push_back(msDuration);
}

void BenchmarkAbstract::addSudokuString(const std::string &sudokuString) {
    sudoku_tests_as_string.push_back(sudokuString);
}