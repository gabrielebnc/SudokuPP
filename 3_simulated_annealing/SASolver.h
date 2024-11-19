#ifndef SUDOKUPP_SASOLVER_H
#define SUDOKUPP_SASOLVER_H

#include <vector>
#include <bitset>

constexpr int SUDOKU_SIZE = 9;

// Best params found (maybe)
// TODO grid search to find best params
constexpr double SA_SOLVER_ALPHA = 0.9995;
constexpr int SA_SOLVER_MAX_ITER = 500'000;

constexpr int DEAFULT_INITIAL_TEMP_N_STATES = 30;

class SASolver {
private:
    std::vector<std::vector<int>> startingBoard;
    std::vector<std::vector<int>> currentBoard;


    [[maybe_unused]] bool setCell(int row, int col, int val);

    static void display_board(const std::vector<std::vector<int>> &board);

    static int row_violations(const std::vector<std::vector<int>> &sudoku);

    [[maybe_unused]] static int col_violations(const std::vector<std::vector<int>> &sudoku);

    [[maybe_unused]] static int cell_violations(const std::vector<std::vector<int>> &sudoku);

    /**
     * Sudoku Evaluation
     * only uses row violations, making the execution faster
     * */
    static int sudoku_evaluation(const std::vector<std::vector<int>> &sudoku);


    std::bitset<SUDOKU_SIZE> seen_elements_cell(int startRow, int startCol);

    void random_fill_cell(int startRow, int startCol);

    void random_fill_sudoku();

    static std::pair<int, int> pick_random_cell();

    void swap_pair_in_cell(const std::pair<int, int> &cell);

    static bool acceptance_function(double T, double delta_eval);


public:
    explicit SASolver(const std::vector<std::vector<int>> &board);

    void reset();

    int current_sudoku_evaluation();

    /**
     * Standard deviation of a number of randomly initialized states
     * */
    int starting_temperature(int n_states = DEAFULT_INITIAL_TEMP_N_STATES);

    void compute_neighbor_position();

    /**
     * Simulated Annealing solving
     *
     * @param iT Initial Temperature for the Simulated Annealing
     * @param alpha Cooling Factor for the Simulated Annealing
     * @param iterations Maximum iterations for the Simulated Annealing
     * @return True if operation was successful, reaching objective status 0
     * */
    bool solve(double iT, double alpha, int iterations);

    [[maybe_unused]] void display_starting_board();

    [[maybe_unused]] void display_current_board();

    [[maybe_unused]] void display_random_filled_board();

};


#endif //SUDOKUPP_SASOLVER_H
