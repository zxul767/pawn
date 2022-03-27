#include <ctime>
#include <functional>
#include <iostream>

#include "../../catch.hpp"
#include "bitboard.hpp"

namespace
{
using bits::random_bitboard;
const uint SQUARES = 64;

TEST_CASE("Random bitboards have few collisions", "[random]")
{
    const uint TRIALS = 500;
    const double COLLISIONS_TOLERANCE = 0.001; // 0.1%

    uint collisions = 0;
    for (uint ones_count = 1; ones_count < SQUARES; ones_count++)
        for (uint trials = 0; trials < TRIALS; trials++)
        {
            bits::bitboard a = random_bitboard(ones_count);
            bits::bitboard b = random_bitboard(ones_count);
            collisions += (a == b);
        }
    REQUIRE(collisions < uint(TRIALS * SQUARES * COLLISIONS_TOLERANCE));
}

// Testing exhaustively is too slow, so we'll have to make do with a
// random sample of numbers for each count of ones
TEST_CASE("bits::count_ones (random)", "[bits][random]")
{
    const uint TRIALS = 1000;
    for (uint ones_count = 1; ones_count < SQUARES; ones_count++)
        for (uint trials = 0; trials < TRIALS; trials++)
        {
            bits::bitboard value = random_bitboard(ones_count);
            uint result = bits::count_ones(value);
            REQUIRE(result == bits::count_ones_baseline(value));
            REQUIRE(result == bits::count_ones_shift(value));
            REQUIRE(result == bits::count_ones_adaptive(value));
            REQUIRE(result == bits::count_ones_std(value));
        }
}

TEST_CASE("bits::count_ones (smoke)", "[bits][smoke]")
{
    REQUIRE(bits::count_ones(0) == 0);
    REQUIRE(bits::count_ones(0b0001) == 1);
    REQUIRE(bits::count_ones(0b0010) == 1);
    REQUIRE(bits::count_ones(0b0011) == 2);
    REQUIRE(bits::count_ones(0b0100) == 1);
    REQUIRE(bits::count_ones(0b0101) == 2);
    REQUIRE(bits::count_ones(0b0110) == 2);
    REQUIRE(bits::count_ones(0b0111) == 3);
    REQUIRE(bits::count_ones(0b1000) == 1);
}

TEST_CASE("Find position of the least significant bit (smoke)", "[bits][lsb][smoke]")
{
    REQUIRE(bits::lsb_position(0) == -1);
    REQUIRE(bits::lsb_position(0b0001) == 0);
    REQUIRE(bits::lsb_position(0b0010) == 1);
    REQUIRE(bits::lsb_position(0b0011) == 0);
    REQUIRE(bits::lsb_position(0b0100) == 2);
    REQUIRE(bits::lsb_position(0b0101) == 0);
    REQUIRE(bits::lsb_position(0b0110) == 1);
    REQUIRE(bits::lsb_position(0b0111) == 0);
    REQUIRE(bits::lsb_position(0b1000) == 3);
}

TEST_CASE("Find position of the most significant bit (smoke)", "[bits][msb][smoke]")
{
    REQUIRE(bits::msb_position(0) == -1);
    REQUIRE(bits::msb_position(0b0001) == 0);
    REQUIRE(bits::msb_position(0b0010) == 1);
    REQUIRE(bits::msb_position(0b0011) == 1);
    REQUIRE(bits::msb_position(0b0100) == 2);
    REQUIRE(bits::msb_position(0b0101) == 2);
    REQUIRE(bits::msb_position(0b0110) == 2);
    REQUIRE(bits::msb_position(0b0111) == 2);
    REQUIRE(bits::msb_position(0b1000) == 3);
}

} // anonymous namespace
