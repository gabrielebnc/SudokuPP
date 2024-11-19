# Sudoku Plus Plus
A collection of Sudoku Solvers, attempting to increase the performance each time (while hopefully learning C++)

## Implemented Solvers

### 1. Naive Backtracking
The first solver implements a basic backtracking technique, brute forcing the puzzle until the solution is found.\
[More](./1_backtracking_naive/README.md)

### 2. Backtracking with Bitset and Array (or BTBitset)
Spin-off of Naive Backtracking, this time using the `std::bitset` and C-style arrays to cut down on execution time.\
[More](./2_backtracking_bitset_array/README.md)

### 3. Simulated Annealing
The third solver explores a probabilistic optimization approach using [Simulated Annealing](https://en.wikipedia.org/wiki/Simulated_annealing).\
[More](./3_simulated_annealing/README.md)