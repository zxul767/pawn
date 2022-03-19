#include "Util.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace util
{
ushort bit(ushort n)
{
    return 1 << n;
}

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

std::vector<bitboard> create_square_to_bitboard()
{
    std::vector<bitboard> result;
    for (uint i = 0; i < constants::SQUARES; ++i)
        result.push_back(constants::ONE << i);
    return result;
}

/*==============================================================================
  Get the position of the most significat bit in BITVECTOR (The algorithm used
  is based on binary search, see LMSBP_algorithm.txt for a description)

  Precondition: bitboard is a 64-bit integer type.
  ==============================================================================*/
int msb_position(bitboard bits)
{
    // The following commented code is the non-optimized version of the code below
    //
    // static u_int shift[] = { 32, 16, 8, 4, 2, 1 };
    // static bitboard masks[] = { 0xFFFFFFFF00000000LL,
    //                             0x00000000FFFF0000LL,
    //                             0x000000000000FF00LL,
    //                             0x00000000000000F0LL,
    //                             0x000000000000000CLL,
    //                             0x0000000000000002LL };
    //
    // if (bits == 0) return -1;
    //
    // u_int position = 64;
    // for (u_int i = 0; i < 6; ++i)
    //    if (masks[i] & bits)
    //       bits >>= shift[i];
    //    else
    //       position -= shift[i];
    //
    // return position-1;

    if (bits == 0)
        return -1;

    int position = 0;

    if (bits & (~0uLL << bit(5)))
    {
        position |= bit(5);
        bits >>= bit(5);
    }
    if (bits & (~0uLL << bit(4)))
    {
        position |= bit(4);
        bits >>= bit(4);
    }
    if (bits & (~0uLL << bit(3)))
    {
        position |= bit(3);
        bits >>= bit(3);
    }
    if (bits & (~0uLL << bit(2)))
    {
        position |= bit(2);
        bits >>= bit(2);
    }
    if (bits & (~0uLL << bit(1)))
    {
        position |= bit(1);
        bits >>= bit(1);
    }
    if (bits & (~0uLL << bit(0)))
    {
        position |= bit(0);
    }
    return position;
}

/*==============================================================================
  | Get the position of the least significat bit in BITVECTOR (The algorithm    |
  | used is based on binary search, see LMSBP_algorithm.txt for a description)  |
  |                                                                             |
  | Precondition: bitboard is a 64-bit integer type.                            |
  ==============================================================================*/
int lsb_position(bitboard bits)
{
    // The following commented code is the non-optimized version of the code below
    //
    // static u_int shift[] = { 32, 16, 8, 4, 2, 1 };
    // static bitboard masks[] = { 0x00000000FFFFFFFFLL,
    //                             0x000000000000FFFFLL,
    //                             0x00000000000000FFLL,
    //                             0x000000000000000FLL,
    //                             0x0000000000000003LL,
    //                             0x0000000000000001LL };
    // if (bits == 0) return -1;
    // u_int position = 1;
    // for (u_int i = 0; i < 6; ++i)
    //    if (!(masks[i] & bits))
    //    {
    //       bits >>= shift[i];
    //       position += shift[i];
    //    }
    // return position-1;

    if (bits == 0)
        return -1;

    int position = 0;

    if (!(bits & (~0uLL >> bit(5))))
    {
        position |= bit(5);
        bits >>= bit(5);
    }
    if (!(bits & (~0uLL >> (bit(4) | bit(5)))))
    {
        position |= bit(4);
        bits >>= bit(4);
    }
    if (!(bits & (~0uLL >> (bit(3) | bit(4) | bit(5)))))
    {
        position |= bit(3);
        bits >>= bit(3);
    }
    if (!(bits & (~0uLL >> (bit(2) | bit(3) | bit(4) | bit(5)))))
    {
        position |= bit(2);
        bits >>= bit(2);
    }
    if (!(bits & (~0uLL >> (bit(1) | bit(2) | bit(3) | bit(4) | bit(5)))))
    {
        position |= bit(1);
        bits >>= bit(1);
    }
    if (!(bits & (~0uLL >> (bit(0) | bit(1) | bit(2) | bit(3) | bit(4) | bit(5)))))
    {
        position |= bit(0);
    }
    return position;
}

/*==============================================================================
  Complexity: O(n) where n is the number of turned-on bits

  Note: There are at least two faster methods to do this task:

  (a) Using a precomputed table of size 2^8 to count the number of bits in
  blocks of eight bits. The code required by this method is rather compact,
  but requires a prior initialization. Complexity is O(1) both for space and
  time.

  (b) Using binary counters of size 1, 2, 4, ..., 2^64. The code used by this
  method is rather compact, but without an explanation it is completely
  cryptic. Complexity is O(1)

  Precondition: bitboard is a 64-bit integer type.
  ==============================================================================*/
uint count_set_bits(bitboard bits)
{
    // The following commented code is the non-optimized version of the code below
    //
    //    uint n_bits = 0;
    //
    //    while (bits)
    //    {
    //       n_bits++;
    //       bits &= bits - 1;
    //    }
    //    return n_bits;
    //
    // We are now using method (b)

    bits = (((0xAAAAAAAAAAAAAAAAuLL & bits) >> 0x1) + ((0x5555555555555555uLL & bits)));
    bits = (((0xCCCCCCCCCCCCCCCCuLL & bits) >> 0x2) + ((0x3333333333333333uLL & bits)));
    bits = (((0xF0F0F0F0F0F0F0F0uLL & bits) >> 0x4) + ((0x0F0F0F0F0F0F0F0FuLL & bits)));
    bits = (((0xFF00FF00FF00FF00uLL & bits) >> 0x8) + ((0x00FF00FF00FF00FFuLL & bits)));
    bits = (((0xFFFF0000FFFF0000uLL & bits) >> 0x10) + ((0x0000FFFF0000FFFFuLL & bits)));
    bits = (((0xFFFFFFFF00000000uLL & bits) >> 0x20) + ((0x00000000FFFFFFFFuLL & bits)));

    return (uint)bits;
}

} // namespace util
