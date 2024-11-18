#ifndef SUDOKUPP_NAIVEBENCHMARK_H
#define SUDOKUPP_NAIVEBENCHMARK_H

#include <vector>
#include <string>
#include "BenchmarkAbstract.h"

class NaiveBenchmark : public BenchmarkAbstract {
public:
    explicit NaiveBenchmark();

    static std::vector<std::vector<int>> string_to_sudoku(const std::string &sudoku_string);

    void run_benchmark() override ;
};

#endif //SUDOKUPP_NAIVEBENCHMARK_H
