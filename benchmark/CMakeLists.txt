# benchmark/CMakeLists.txt

# Step 1: Create the 'benchmark' library
add_library(benchmark STATIC BenchmarkAbstract.cpp)

# Step 2: Link the 'benchmark' library to 'sudoku_utils'
target_link_libraries(benchmark PRIVATE sudoku_utils)

# Step 3: Specify the include directory for 'benchmark'
# This should allow 'benchmark' to find necessary headers from the current directory
target_include_directories(benchmark PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
