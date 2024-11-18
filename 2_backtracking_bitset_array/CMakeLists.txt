add_executable(2_backtracking_bitset_array main.cpp
        backtracking_bitset_array.cpp
        backtracking_bitset_array.h
        BTBitsetBenchmark.cpp
        BTBitsetBenchmark.h
)


target_link_libraries(2_backtracking_bitset_array PRIVATE benchmark)