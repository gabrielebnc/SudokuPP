cmake_minimum_required(VERSION 3.27)
project(SudokuPP)

set(CMAKE_CXX_STANDARD 17)

# Benchmarking
add_subdirectory(benchmark)

# Individual Projects
add_subdirectory(1_backtracking_naive)
add_subdirectory(2_backtracking_bitset_array)
add_subdirectory(3_simulated_annealing)