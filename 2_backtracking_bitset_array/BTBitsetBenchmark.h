#ifndef SUDOKUPP_BTBITSETBENCHMARK_H
#define SUDOKUPP_BTBITSETBENCHMARK_H

#include <vector>
#include <string>
#include "BenchmarkAbstract.h"
#include "backtracking_bitset_array.h"

class BTBitsetBenchmark : public BenchmarkAbstract {
public:
    explicit BTBitsetBenchmark();


    static int (&string_to_sudoku(const std::string &sudoku_string,
                                  int (&sudoku)[SUDOKU_SIZE][SUDOKU_SIZE]))[SUDOKU_SIZE][SUDOKU_SIZE];

    void run_benchmark() override;
};

#endif //SUDOKUPP_BTBITSETBENCHMARK_H
