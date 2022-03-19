#ifndef UTIL_H
#define UTIL_H

#include <climits>
#include <cstddef>
#include <vector>

typedef unsigned short int ushort;
typedef unsigned int uint;
typedef unsigned long long ullong;

namespace util
{
typedef unsigned long long bitboard;

namespace constants
{
constexpr int INFINITUM = INT_MAX / 2;
constexpr double EPSILON = 1E-6;
constexpr bitboard ONE = 1;
constexpr uint SQUARES = 64;

} // namespace constants

std::vector<bitboard> create_square_to_bitboard();
const std::vector<bitboard> to_bitboard = create_square_to_bitboard();

int lsb_position(bitboard bits);
int msb_position(bitboard bits);
uint count_set_bits(bitboard bits);
double random(double low, double high);
ullong random_ullong();
bool is_odd(uint n);

} // namespace util

#endif // UTIL_H
