#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <numeric>
#include "BenchmarkAbstract.h"

const int SUDOKU_SIZE = 9;
const int SUDOKU_CELL_SIZE = 3;

/*
 * Utility function to display the sudoku
 * */
void display_sudoku(const std::vector<std::vector<int>> &sudoku) {
    for (int i = 0; i < SUDOKU_SIZE; i++) {
        for (int j = 0; j < SUDOKU_SIZE; j++) {
            if (j % 3 == 0) std::cout << "|";
            std::cout << " " << sudoku[i][j] << " ";
        }
        std::cout << "|\n";
    }
}

/*
 * Detects violation in sudoku's rows
 * */
bool is_row_violated(const std::vector<std::vector<int>> &sudoku) {
    for (const auto &row: sudoku) {
        std::set<int> seen;
        for (auto num: row) {
            if (num == 0) continue;
            if (seen.find(num) != seen.end()) {
                return true;
            }
            seen.insert(num);
        }
    }
    return false;
}

/*
 * Detects violation in sudoku's columns
 * */
bool is_col_violated(const std::vector<std::vector<int>> &sudoku) {
    for (int i = 0; i < SUDOKU_SIZE; ++i) {
        std::set<int> seen;
        for (int j = 0; j < SUDOKU_SIZE; ++j) {
            int elem = sudoku[j][i];
            if (elem == 0) continue;
            if (seen.find(elem) != seen.end()) {
                return true;
            }
            seen.insert(elem);
        }
    }
    return false;
}

/*
 * Detects violation in sudoku's given cell
 * */
bool is_cell_violated(const std::vector<std::vector<int>> &sudoku, int startRow, int startCol) {
    std::set<int> seen;
    for (int row = startRow; row < startRow + SUDOKU_CELL_SIZE; ++row) {
        for (int col = startCol; col < startCol + SUDOKU_CELL_SIZE; ++col) {
            int elem = sudoku[row][col];
            if (elem == 0) continue;
            if (seen.find(elem) != seen.end()) {
                return true;
            }
            seen.insert(elem);
        }
    }
    return false;
}

/*
 * Detects violation in sudoku's cells
 * */
bool is_cells_violated(const std::vector<std::vector<int>> &sudoku) {
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
bool is_sudoku_violated(const std::vector<std::vector<int>> &sudoku) {
    return is_row_violated(sudoku) || is_col_violated(sudoku) || is_cells_violated(sudoku);
}

/*
 * Sudoku solver with backtracking
 * Modifies the sudoku in-place
 * Returns wether the operation is successful or not
 * */
bool solve_sudoku(std::vector<std::vector<int>> &sudoku) {

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

class NaiveBenchmark : public BenchmarkAbstract {
public:
    explicit NaiveBenchmark(const std::filesystem::path &sudoku_tests_path)
            : BenchmarkAbstract(sudoku_tests_path) {
    }

    static std::vector<std::vector<int>> string_to_sudoku(const std::string &sudoku_string) {
        std::vector<std::vector<int>> sudoku(9, std::vector<int>(9));
        for (auto i = 0; i < sudoku_string.length(); ++i) {
            int row = i / SUDOKU_SIZE;
            int col = i % SUDOKU_SIZE;
            sudoku[row][col] = handle_char(sudoku_string[i]);
        }
        return sudoku;
    }

    void run_benchmark() override {
        for (const auto &sudoku_string: getSudokuTestsAsString()) {
            auto sudoku = string_to_sudoku(sudoku_string);
            auto start = std::chrono::high_resolution_clock::now();
            solve_sudoku(sudoku);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            addMsDuration(duration.count());
        }

        print_benchmark_results( "Naive");
    }
};

int main() {
    std::filesystem::path benchmark_file = "../../benchmark/sudoku_benchmark_test.txt";
    NaiveBenchmark bench(benchmark_file);
    bench.run_benchmark();
}
