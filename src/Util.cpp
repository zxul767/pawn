#include "Util.hpp"
#include <cstdlib>
#include <iostream>

namespace util
{
bool is_odd(uint n)
{
    return n % 2 == 1;
}

std::vector<bitboard> Util::to_bitboard;

bool Util::loaded = Util::load_to_bitboard();

bool Util::load_to_bitboard()
{
    const static uint SQUARES = 64;
    bitboard one = 1;

    for (uint i = 0; i < SQUARES; ++i)
        to_bitboard.push_back(one << i);

    return true;
}

/*==============================================================================
  Get the position of the most significat bit in BITVECTOR (The algorithm used
  is based on binary search, see LMSBP_algorithm.txt for a description)

  Precondition: bitboard is a 64-bit integer type.
  ==============================================================================*/
int Util::MSB_position(bitboard bitvector)
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
    // if (bitvector == 0) return -1;
    //
    // u_int position = 64;
    // for (u_int i = 0; i < 6; ++i)
    //    if (masks[i] & bitvector)
    //       bitvector >>= shift[i];
    //    else
    //       position -= shift[i];
    //
    // return position-1;

    if (bitvector == 0)
        return -1;

    int position = 0;

    if (bitvector & (~0uLL << bit(5)))
    {
        position |= bit(5);
        bitvector >>= bit(5);
    }
    if (bitvector & (~0uLL << bit(4)))
    {
        position |= bit(4);
        bitvector >>= bit(4);
    }
    if (bitvector & (~0uLL << bit(3)))
    {
        position |= bit(3);
        bitvector >>= bit(3);
    }
    if (bitvector & (~0uLL << bit(2)))
    {
        position |= bit(2);
        bitvector >>= bit(2);
    }
    if (bitvector & (~0uLL << bit(1)))
    {
        position |= bit(1);
        bitvector >>= bit(1);
    }
    if (bitvector & (~0uLL << bit(0)))
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
int Util::LSB_position(bitboard bitvector)
{
    // The following commented code is the non-optimized version of the code below
    //
    //   static u_int shift[] = { 32, 16, 8, 4, 2, 1 };
    // static bitboard masks[] = { 0x00000000FFFFFFFFLL,
    //                             0x000000000000FFFFLL,
    //                             0x00000000000000FFLL,
    //                             0x000000000000000FLL,
    //                             0x0000000000000003LL,
    //                             0x0000000000000001LL };

    // if (bitvector == 0) return -1;

    // u_int position = 1;
    // for (u_int i = 0; i < 6; ++i)
    //    if (!(masks[i] & bitvector))
    //    {
    //       bitvector >>= shift[i];
    //       position += shift[i];
    //    }

    // return position-1;

    if (bitvector == 0)
        return -1;

    int position = 0;

    if (!(bitvector & (~0uLL >> bit(5))))
    {
        position |= bit(5);
        bitvector >>= bit(5);
    }
    if (!(bitvector & (~0uLL >> (bit(4) | bit(5)))))
    {
        position |= bit(4);
        bitvector >>= bit(4);
    }
    if (!(bitvector & (~0uLL >> (bit(3) | bit(4) | bit(5)))))
    {
        position |= bit(3);
        bitvector >>= bit(3);
    }
    if (!(bitvector & (~0uLL >> (bit(2) | bit(3) | bit(4) | bit(5)))))
    {
        position |= bit(2);
        bitvector >>= bit(2);
    }
    if (!(bitvector & (~0uLL >> (bit(1) | bit(2) | bit(3) | bit(4) | bit(5)))))
    {
        position |= bit(1);
        bitvector >>= bit(1);
    }
    if (!(bitvector & (~0uLL >> (bit(0) | bit(1) | bit(2) | bit(3) | bit(4) | bit(5)))))
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
uint Util::count_set_bits(bitboard bitvector)
{
    // The following commented code is the non-optimized version of the code below
    //
    //    uint n_bits = 0;
    //
    //    while (bitvector)
    //    {
    //       n_bits++;
    //       bitvector &= bitvector - 1;
    //    }
    //    return n_bits;

    // We are now using method (b)

    bitvector =
        (((0xAAAAAAAAAAAAAAAAuLL & bitvector) >> 0x1) +
         ((0x5555555555555555uLL & bitvector)));

    bitvector =
        (((0xCCCCCCCCCCCCCCCCuLL & bitvector) >> 0x2) +
         ((0x3333333333333333uLL & bitvector)));

    bitvector =
        (((0xF0F0F0F0F0F0F0F0uLL & bitvector) >> 0x4) +
         ((0x0F0F0F0F0F0F0F0FuLL & bitvector)));

    bitvector =
        (((0xFF00FF00FF00FF00uLL & bitvector) >> 0x8) +
         ((0x00FF00FF00FF00FFuLL & bitvector)));

    bitvector =
        (((0xFFFF0000FFFF0000uLL & bitvector) >> 0x10) +
         ((0x0000FFFF0000FFFFuLL & bitvector)));

    bitvector =
        (((0xFFFFFFFF00000000uLL & bitvector) >> 0x20) +
         ((0x00000000FFFFFFFFuLL & bitvector)));

    return (uint)bitvector;
}

ullong Util::random_ullong()
{
    return (
        ((ullong)rand()) ^ ((ullong)rand() << 15) ^ ((ullong)rand() << 30) ^
        ((ullong)rand() << 45) ^ ((ullong)rand() << 60));
}

void Util::to_binary(ullong value)
{
    if (value > 0)
    {
        to_binary(value >> 1);
        std::cout << (value & 1 ? '1' : '0');
    }
}

double Util::random(double low, double high)
{
    double tmp;

    if (low > high)
    {
        tmp = low;
        low = high;
        high = tmp;
    }

    tmp = (rand() / static_cast<double>(RAND_MAX)) * (high - low) + low;
    return tmp;
}

} // namespace util
