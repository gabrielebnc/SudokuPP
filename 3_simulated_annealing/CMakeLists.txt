add_executable(3_simulated_annealing main.cpp
        SASolver.cpp
        SASolver.h
        SABenchmark.cpp
        SABenchmark.h)

# Step 2: Link the 'benchmark' library to 'sudoku_utils'
target_link_libraries(3_simulated_annealing PRIVATE sudoku_utils benchmark)