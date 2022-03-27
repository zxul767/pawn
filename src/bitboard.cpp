#include "bitboard.hpp"

#include <algorithm>
#include <bitset>
#include <random>

namespace bits
{
static ushort pow2(ushort n)
{
    return 1 << n;
}

// TODO: is this really random enough?
static std::vector<uint> create_shuffled_range(uint size)
{
    std::vector<uint> indices(size);
    std::iota(indices.begin(), indices.end(), 0);

    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(indices.begin(), indices.end(), generator);

    return indices;
}

// This has proven to be the fastest implementation. However, contrary to what
// I thought at first, the difference between this and the implementations below
// is not really that big. You can test this yourself by running from your shell:
//
// > make perf_test
//
uint count_ones(bitboard bits)
{
    bits = (((0xAAAAAAAAAAAAAAAAuLL & bits) >> 0x1) + ((0x5555555555555555uLL & bits)));
    bits = (((0xCCCCCCCCCCCCCCCCuLL & bits) >> 0x2) + ((0x3333333333333333uLL & bits)));
    bits = (((0xF0F0F0F0F0F0F0F0uLL & bits) >> 0x4) + ((0x0F0F0F0F0F0F0F0FuLL & bits)));
    bits = (((0xFF00FF00FF00FF00uLL & bits) >> 0x8) + ((0x00FF00FF00FF00FFuLL & bits)));
    bits = (((0xFFFF0000FFFF0000uLL & bits) >> 0x10) + ((0x0000FFFF0000FFFFuLL & bits)));
    bits = (((0xFFFFFFFF00000000uLL & bits) >> 0x20) + ((0x00000000FFFFFFFFuLL & bits)));

    return (uint)bits;
}

uint count_ones_std(bitboard bits)
{
    return std::bitset<BITS_IN_BITBOARD>(bits).count();
}

uint count_ones_baseline(bitboard bits)
{
    uint count = 0;
    for (uint i = 0; i < BITS_IN_BITBOARD; i++)
        if (bits & (ONE << i))
            count++;
    return count;
}

uint count_ones_shift(bitboard bits)
{
    uint count = 0;
    while (bits)
    {
        count += bits & 1;
        bits >>= 1;
    }
    return count;
}

uint count_ones_adaptive(bitboard bits)
{
    uint count = 0;
    while (bits)
    {
        count++;
        bits &= bits - 1;
    }
    return count;
}

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

    if (bits & (~0uLL << pow2(5)))
    {
        position |= pow2(5);
        bits >>= pow2(5);
    }
    if (bits & (~0uLL << pow2(4)))
    {
        position |= pow2(4);
        bits >>= pow2(4);
    }
    if (bits & (~0uLL << pow2(3)))
    {
        position |= pow2(3);
        bits >>= pow2(3);
    }
    if (bits & (~0uLL << pow2(2)))
    {
        position |= pow2(2);
        bits >>= pow2(2);
    }
    if (bits & (~0uLL << pow2(1)))
    {
        position |= pow2(1);
        bits >>= pow2(1);
    }
    if (bits & (~0uLL << pow2(0)))
    {
        position |= pow2(0);
    }
    return position;
}

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

    if (!(bits & (~0uLL >> pow2(5))))
    {
        position |= pow2(5);
        bits >>= pow2(5);
    }
    if (!(bits & (~0uLL >> (pow2(4) | pow2(5)))))
    {
        position |= pow2(4);
        bits >>= pow2(4);
    }
    if (!(bits & (~0uLL >> (pow2(3) | pow2(4) | pow2(5)))))
    {
        position |= pow2(3);
        bits >>= pow2(3);
    }
    if (!(bits & (~0uLL >> (pow2(2) | pow2(3) | pow2(4) | pow2(5)))))
    {
        position |= pow2(2);
        bits >>= pow2(2);
    }
    if (!(bits & (~0uLL >> (pow2(1) | pow2(2) | pow2(3) | pow2(4) | pow2(5)))))
    {
        position |= pow2(1);
        bits >>= pow2(1);
    }
    if (!(bits & (~0uLL >> (pow2(0) | pow2(1) | pow2(2) | pow2(3) | pow2(4) | pow2(5)))))
    {
        position |= pow2(0);
    }
    return position;
}

std::vector<bitboard> create_square_to_bitboard_map()
{
    bitboard one = 1;
    std::vector<bitboard> result;
    for (uint i = 0; i < BITS_IN_BITBOARD; ++i)
        result.push_back(one << i);
    return result;
}

bitboard random_bitboard(uint ones_count)
{
    // We want to generate a bitboard with `ones_count` 1s at random positions
    //
    // This is equivalent to finding a random subset of {0..SQUARES}
    // with size `ones_count`.
    //
    // One way to accomplish this is to generate a random permutation
    // of all indices and then take the first `ones_count`
    //
    std::vector<uint> indices = create_shuffled_range(8 * sizeof(bitboard));
    bits::bitboard result = 0;
    for (uint i = 0; i < ones_count; i++)
        result |= (bits::ONE << indices[i]);
    return result;
}

} // namespace bits
