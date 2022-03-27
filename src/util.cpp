#include "util.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace util
{
bool is_odd(uint n)
{
    return n % 2 == 1;
}

ullong random_ullong()
{
    return (
        ((ullong)rand()) ^ ((ullong)rand() << 15) ^ ((ullong)rand() << 30) ^
        ((ullong)rand() << 45) ^ ((ullong)rand() << 60));
}

double random(double low, double high)
{
    if (low > high)
    {
        std::swap(low, high);
    }
    return (rand() / static_cast<double>(RAND_MAX)) * (high - low) + low;
}

} // namespace util
