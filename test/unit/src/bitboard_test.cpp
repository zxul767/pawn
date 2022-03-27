#include <ctime>
#include <functional>
#include <iostream>

#include "../../catch.hpp"
#include "bitboard.hpp"

namespace
{
using namespace bits;

const uint SQUARES = 64;

TEST_CASE("Random bitboards have few collisions", "[bits][random]")
{
    const uint TRIALS = 500;
    const double COLLISIONS_TOLERANCE = 0.001; // 0.1%

    uint collisions = 0;
    for (uint ones_count = 1; ones_count < SQUARES; ones_count++)
        for (uint trials = 0; trials < TRIALS; trials++)
        {
            bitboard a = random_bitboard(ones_count);
            bitboard b = random_bitboard(ones_count);
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
            bitboard value = random_bitboard(ones_count);
            uint result = count_ones(value);
            REQUIRE(result == count_ones_baseline(value));
            REQUIRE(result == count_ones_shift(value));
            REQUIRE(result == count_ones_adaptive(value));
            REQUIRE(result == count_ones_std(value));
        }
}

TEST_CASE("bits::count_ones (smoke)", "[bits][smoke]")
{
    REQUIRE(count_ones(0) == 0);
    REQUIRE(count_ones(0b0001) == 1);
    REQUIRE(count_ones(0b0010) == 1);
    REQUIRE(count_ones(0b0011) == 2);
    REQUIRE(count_ones(0b0100) == 1);
    REQUIRE(count_ones(0b0101) == 2);
    REQUIRE(count_ones(0b0110) == 2);
    REQUIRE(count_ones(0b0111) == 3);
    REQUIRE(count_ones(0b1000) == 1);
}

TEST_CASE("bits::lsb_position (smoke)", "[bits][lsb][smoke]")
{
    REQUIRE(lsb_position(0) == -1);
    REQUIRE(lsb_position(0b0001) == 0);
    REQUIRE(lsb_position(0b0010) == 1);
    REQUIRE(lsb_position(0b0011) == 0);
    REQUIRE(lsb_position(0b0100) == 2);
    REQUIRE(lsb_position(0b0101) == 0);
    REQUIRE(lsb_position(0b0110) == 1);
    REQUIRE(lsb_position(0b0111) == 0);
    REQUIRE(lsb_position(0b1000) == 3);
}

TEST_CASE("bits::msb_position (smoke)", "[bits][msb][smoke]")
{
    REQUIRE(msb_position(0) == -1);
    REQUIRE(msb_position(0b0001) == 0);
    REQUIRE(msb_position(0b0010) == 1);
    REQUIRE(msb_position(0b0011) == 1);
    REQUIRE(msb_position(0b0100) == 2);
    REQUIRE(msb_position(0b0101) == 2);
    REQUIRE(msb_position(0b0110) == 2);
    REQUIRE(msb_position(0b0111) == 2);
    REQUIRE(msb_position(0b1000) == 3);
}

} // anonymous namespace
