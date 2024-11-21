# 3. Simulated Annealing

**Simulated annealing is a probabilistic optimization algorithm inspired by the annealing process in metallurgy, which iteratively explores the solution space by accepting both improving and worsening moves based on a temperature parameter that gradually decreases over time.**\
For more info: [Wikipedia](https://en.wikipedia.org/wiki/Simulated_annealing), 

## Criticalities and Questions

- Can be more efficient?
- Not deterministic
- Can terminate with wrong result
- Can parallelism be used?

## Performance (from benchmark execution)

BTBitset results:\
Executed Benchmark on 12 testing puzzles.\
All results are correct.\
Average execution: 167.417ms = 0.167417s.\
Standard deviation: 17.816

Results correctness is not 100% guaranteed, there might be around a 0.7-1% chance of incorrect result.\
This depends on the Simulated Annealing parameters (initial temperature, cooling rate and max iterations).
