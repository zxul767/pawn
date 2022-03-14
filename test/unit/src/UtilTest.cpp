#include "Util.hpp"
#include "catch.hpp"

namespace
{
using util::Util;

TEST_CASE("Count number of set bits", "[bits]")
{
    REQUIRE(Util::count_set_bits(0) == 0);
    REQUIRE(Util::count_set_bits(1) == 1); // binary: 0001
    REQUIRE(Util::count_set_bits(2) == 1); // binary: 0010
    REQUIRE(Util::count_set_bits(3) == 2); // binary: 0011
    REQUIRE(Util::count_set_bits(4) == 1); // binary: 0100
    REQUIRE(Util::count_set_bits(5) == 2); // binary: 0101
    REQUIRE(Util::count_set_bits(6) == 2); // binary: 0110
    REQUIRE(Util::count_set_bits(7) == 3); // binary: 0111
    REQUIRE(Util::count_set_bits(8) == 1); // binary: 1000
}

TEST_CASE("Find position of the least significant bit", "[bits][lsb]")
{
    REQUIRE(Util::LSB_position(0) == -1);
    REQUIRE(Util::LSB_position(1) == 0); // binary: 0001
    REQUIRE(Util::LSB_position(2) == 1); // binary: 0010
    REQUIRE(Util::LSB_position(3) == 0); // binary: 0011
    REQUIRE(Util::LSB_position(4) == 2); // binary: 0100
    REQUIRE(Util::LSB_position(5) == 0); // binary: 0101
    REQUIRE(Util::LSB_position(6) == 1); // binary: 0110
    REQUIRE(Util::LSB_position(7) == 0); // binary: 0111
    REQUIRE(Util::LSB_position(8) == 3); // binary: 1000
}

TEST_CASE("Find position of the most significant bit", "[bits][msb]")
{
    REQUIRE(Util::MSB_position(0) == -1);
    REQUIRE(Util::MSB_position(1) == 0); // binary: 0001
    REQUIRE(Util::MSB_position(2) == 1); // binary: 0010
    REQUIRE(Util::MSB_position(3) == 1); // binary: 0011
    REQUIRE(Util::MSB_position(4) == 2); // binary: 0100
    REQUIRE(Util::MSB_position(5) == 2); // binary: 0101
    REQUIRE(Util::MSB_position(6) == 2); // binary: 0110
    REQUIRE(Util::MSB_position(7) == 2); // binary: 0111
    REQUIRE(Util::MSB_position(8) == 3); // binary: 1000
}

} // anonymous namespace
