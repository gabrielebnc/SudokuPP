#include "../include/utils/math.h"

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