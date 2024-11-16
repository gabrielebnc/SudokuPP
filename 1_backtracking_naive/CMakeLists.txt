add_executable(1_backtracking_naive main.cpp
        NaiveBenchmark.cpp
        NaiveBenchmark.h
        main.h)

target_link_libraries(1_backtracking_naive PRIVATE benchmark)