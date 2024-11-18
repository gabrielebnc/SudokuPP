#include <filesystem>
#include "NaiveBenchmark.h"


int main() {
    std::filesystem::path benchmark_file = "../../benchmark/sudoku_benchmark_test.txt";
    NaiveBenchmark bench(benchmark_file);
    bench.run_benchmark();
}
