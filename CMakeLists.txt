# parent CMakeLists.txt

cmake_minimum_required(VERSION 3.27)
project(SudokuPP)

set(CMAKE_CXX_STANDARD 17)
#[[set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -ffast-math -funroll-loops")]]


# Benchmarking
add_subdirectory(utils)
add_subdirectory(benchmark)

# Individual Projects
add_subdirectory(1_backtracking_naive)
add_subdirectory(2_backtracking_bitset_array)
add_subdirectory(3_simulated_annealing)