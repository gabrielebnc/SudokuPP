#ifndef SUDOKUPP_MATH_H
#define SUDOKUPP_MATH_H

#include <numeric>
#include <vector>
#include <cmath>
#include <random>

long double mean(std::vector<long long> samples);

long double standardDeviation(std::vector<long long> samples);


int randomIntInRange(int min, int max);

double randomDoubleInRange(double min, double max);

double percentage(int part, int total);

#endif //SUDOKUPP_MATH_H