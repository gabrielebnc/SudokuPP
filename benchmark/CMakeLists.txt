add_library(benchmark STATIC BenchmarkAbstract.cpp)

# Specify the include directory for other targets
target_include_directories(benchmark PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})