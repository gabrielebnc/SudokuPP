#include "../include/utils/math.h"

[[maybe_unused]] long double mean(std::vector<long long> samples){
    return static_cast<long double>(std::accumulate(samples.begin(), samples.end(), 0LL))/samples.size();
}