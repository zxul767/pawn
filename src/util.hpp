#ifndef UTIL_H
#define UTIL_H

#include "type_aliases.hpp"
#include <climits>

namespace util
{
namespace constants
{
constexpr int INFINITUM = INT_MAX / 2;
constexpr double EPSILON = 1E-6;
constexpr uint SQUARES = 64;

} // namespace constants

double random(double low, double high);
ullong random_ullong();
bool is_odd(uint n);

} // namespace util

#endif // UTIL_H
