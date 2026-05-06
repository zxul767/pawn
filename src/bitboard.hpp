#ifndef BITBOARD_H_
#define BITBOARD_H_

#include "type_aliases.hpp"
#include <vector>

namespace bits
{
using std::vector;
using bitboard = unsigned long long;

constexpr bitboard ONE = 1uLL;
constexpr bitboard ZERO = 0uLL;
constexpr uint BITS_IN_BITBOARD = 8 * sizeof(bitboard);

// all count_ones are functionally equivalent, and vary only in implementation
uint count_ones(bitboard bits);
uint count_ones_std(bitboard bits);
uint count_ones_baseline(bitboard bits);
uint count_ones_shift(bitboard bits);
uint count_ones_adaptive(bitboard bits);

int msb_position(bitboard bits);
int lsb_position(bitboard bits);

inline bitboard as_bitboard(uint position)
{
    return ONE << position;
}

// TODO: add unit tests
inline bitboard turn_off_bit(bitboard &bits, uint position)
{
    return bits &= ~as_bitboard(position);
}

inline bitboard merge_into(bitboard &a, bitboard b)
{
    return a |= b;
}

inline bitboard merge(bitboard a, bitboard b)
{
    return a | b;
}

inline bitboard xor_into(bitboard &a, bitboard b)
{
    return a ^= b;
}

bitboard random_bitboard(uint ones_count);

// TODO: deprecate this data structure in favor of the inline function
// `as_bitboard(...)`
std::vector<bitboard> create_square_to_bitboard_map();
const std::vector<bitboard> to_bitboard = create_square_to_bitboard_map();

} // namespace bits

#endif // BITBOARD_H_
