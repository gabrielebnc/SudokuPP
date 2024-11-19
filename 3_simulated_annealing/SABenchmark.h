#ifndef SUDOKUPP_SABENCHMARK_H
#define SUDOKUPP_SABENCHMARK_H

#include <BenchmarkAbstract.h>

class SABenchmark : public BenchmarkAbstract{

public:
    explicit SABenchmark();

    void run_benchmark() override;

    static std::vector<std::vector<int>> string_to_sudoku(const std::string &sudoku_string);
};


#endif //SUDOKUPP_SABENCHMARK_H
