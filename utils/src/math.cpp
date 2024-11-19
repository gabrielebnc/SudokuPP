#include "../include/utils/math.h"
#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());

long double mean(std::vector<long long> samples) {
    return static_cast<long double>(std::accumulate(samples.begin(), samples.end(), 0LL)) / samples.size();
}

long double standardDeviation(std::vector<long long> samples) {
    if (samples.empty()) {
        return 0.0;
    }
    long double mean = std::accumulate(samples.begin(), samples.end(), 0.0L) / samples.size();
    long double variance = 0.0L;
    for (long long sample : samples) {
        variance += (sample - mean) * (sample - mean);
    }
    variance /= samples.size();

    return std::sqrt(variance);
}


int randomIntInRange(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

double randomDoubleInRange(double min, double max) {
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

double percentage(int part, int total) {
    return total == 0 ? 0.0 : (static_cast<double>(part) / total) * 100.0;
}
