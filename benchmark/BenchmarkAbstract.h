#ifndef SUDOKUPP_BENCHMARKABSTRACT_H
#define SUDOKUPP_BENCHMARKABSTRACT_H

#include <filesystem>
#include <string>
#include <vector>

class BenchmarkAbstract {

public:
    /**
     * Constructor that initializes the path to the Sudoku test file and reads it.
     * @param sudoku_tests_path Path to the Sudoku tests file.
     */
    explicit BenchmarkAbstract(std::filesystem::path sudoku_tests_path);

    /**
     * Pure virtual function to run the benchmark.
     * Derived classes must implement this method to run their specific benchmarking logic.
     */
    virtual void run_benchmark() = 0;

    /**
     * Procedure to read the Sudoku tests from the provided file.
     * The file should contain one flattened Sudoku board per line (81 characters each).
     * @param sudoku_tests_path Path to the Sudoku tests file.
     */
    void read_sudoku_test_file(const std::filesystem::path& sudoku_tests_path);

    std::vector<std::string> get_sudoku_tests_as_string() const;

    /**
     *
     * @param c
     * @return the char converted to the corresponding integer, returns 0 if char is not between 1 and 9 or if it is not
     * an int
     */
    static int handle_char(char c);

private:
    std::filesystem::path path_to_tests; /**< Path to the Sudoku test file */
    std::vector<std::string> sudoku_tests_as_string; /**< Vector containing Sudoku test cases as strings */
};


#endif //SUDOKUPP_BENCHMARKABSTRACT_H
