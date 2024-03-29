#ifndef BITBOARD_H_
#define BITBOARD_H_

#include "type_aliases.hpp"
#include <vector>

namespace bits
{
typedef unsigned long long bitboard;

constexpr bitboard ONE = 1uLL;
constexpr uint BITS_IN_BITBOARD = 8 * sizeof(bitboard);

// all count_ones are functionally equivalent, and vary only in implementation
uint count_ones(bitboard bits);
uint count_ones_std(bitboard bits);
uint count_ones_baseline(bitboard bits);
uint count_ones_shift(bitboard bits);
uint count_ones_adaptive(bitboard bits);

int msb_position(bitboard bits);
int lsb_position(bitboard bits);

std::vector<bitboard> create_square_to_bitboard_map();
const std::vector<bitboard> to_bitboard = create_square_to_bitboard_map();

bitboard random_bitboard(uint ones_count);
} // namespace bits

#endif // BITBOARD_H_
