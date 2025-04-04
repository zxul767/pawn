#include "bitboard.hpp"

#include <algorithm>
#include <bitset>
#include <random>

namespace bits
{
inline static ushort pow2(ushort n)
{
    return 1 << n;
}

static std::vector<uint> create_shuffled_range(uint size)
{
    std::vector<uint> indices(size);
    std::iota(indices.begin(), indices.end(), 0);

    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(indices.begin(), indices.end(), generator);

    return indices;
}

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
        if (bits & as_bitboard(i))
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
    // clang-format off
    static const bitboard masks[] = {
        0xFFFFFFFF00000000LL,
        0x00000000FFFF0000LL,
        0x000000000000FF00LL,
        0x00000000000000F0LL,
        0x000000000000000CLL,
        0x0000000000000002LL,
    };
    // clang-format on

    if (bits == 0)
        return -1;

    int position = 0;

    if (bits & masks[0])
    {
        position |= pow2(5);
        bits >>= pow2(5);
    }
    if (bits & masks[1])
    {
        position |= pow2(4);
        bits >>= pow2(4);
    }
    if (bits & masks[2])
    {
        position |= pow2(3);
        bits >>= pow2(3);
    }
    if (bits & masks[3])
    {
        position |= pow2(2);
        bits >>= pow2(2);
    }
    if (bits & masks[4])
    {
        position |= pow2(1);
        bits >>= pow2(1);
    }
    if (bits & masks[5])
    {
        position |= pow2(0);
    }
    return position;
}

int lsb_position(bitboard bits)
{
    // clang-format off
    static const bitboard masks[] = {
        0x00000000FFFFFFFFLL,
        0x000000000000FFFFLL,
        0x00000000000000FFLL,
        0x000000000000000FLL,
        0x0000000000000003LL,
        0x0000000000000001LL
    };
    // clang-format on
    if (bits == 0)
        return -1;

    int position = 0;

    if (!(bits & masks[0]))
    {
        position |= pow2(5);
        bits >>= pow2(5);
    }
    if (!(bits & masks[1]))
    {
        position |= pow2(4);
        bits >>= pow2(4);
    }
    if (!(bits & masks[2]))
    {
        position |= pow2(3);
        bits >>= pow2(3);
    }
    if (!(bits & masks[3]))
    {
        position |= pow2(2);
        bits >>= pow2(2);
    }
    if (!(bits & masks[4]))
    {
        position |= pow2(1);
        bits >>= pow2(1);
    }
    if (!(bits & masks[5]))
    {
        position |= pow2(0);
    }
    return position;
}

std::vector<bitboard> create_square_to_bitboard_map()
{
    std::vector<bitboard> result;
    for (uint i = 0; i < BITS_IN_BITBOARD; ++i)
        result.push_back(as_bitboard(i));
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
    std::vector<uint> indices = create_shuffled_range(BITS_IN_BITBOARD);
    bits::bitboard result = 0;
    for (uint i = 0; i < ones_count; i++)
        result |= as_bitboard(indices[i]);
    return result;
}

} // namespace bits
