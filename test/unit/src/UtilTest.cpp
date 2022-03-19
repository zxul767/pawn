#include "Util.hpp"
#include "catch.hpp"

namespace
{
TEST_CASE("Count number of set bits", "[bits]")
{
    REQUIRE(util::count_set_bits(0) == 0);
    REQUIRE(util::count_set_bits(1) == 1); // binary: 0001
    REQUIRE(util::count_set_bits(2) == 1); // binary: 0010
    REQUIRE(util::count_set_bits(3) == 2); // binary: 0011
    REQUIRE(util::count_set_bits(4) == 1); // binary: 0100
    REQUIRE(util::count_set_bits(5) == 2); // binary: 0101
    REQUIRE(util::count_set_bits(6) == 2); // binary: 0110
    REQUIRE(util::count_set_bits(7) == 3); // binary: 0111
    REQUIRE(util::count_set_bits(8) == 1); // binary: 1000
}

TEST_CASE("Find position of the least significant bit", "[bits][lsb]")
{
    REQUIRE(util::lsb_position(0) == -1);
    REQUIRE(util::lsb_position(1) == 0); // binary: 0001
    REQUIRE(util::lsb_position(2) == 1); // binary: 0010
    REQUIRE(util::lsb_position(3) == 0); // binary: 0011
    REQUIRE(util::lsb_position(4) == 2); // binary: 0100
    REQUIRE(util::lsb_position(5) == 0); // binary: 0101
    REQUIRE(util::lsb_position(6) == 1); // binary: 0110
    REQUIRE(util::lsb_position(7) == 0); // binary: 0111
    REQUIRE(util::lsb_position(8) == 3); // binary: 1000
}

TEST_CASE("Find position of the most significant bit", "[bits][msb]")
{
    REQUIRE(util::msb_position(0) == -1);
    REQUIRE(util::msb_position(1) == 0); // binary: 0001
    REQUIRE(util::msb_position(2) == 1); // binary: 0010
    REQUIRE(util::msb_position(3) == 1); // binary: 0011
    REQUIRE(util::msb_position(4) == 2); // binary: 0100
    REQUIRE(util::msb_position(5) == 2); // binary: 0101
    REQUIRE(util::msb_position(6) == 2); // binary: 0110
    REQUIRE(util::msb_position(7) == 2); // binary: 0111
    REQUIRE(util::msb_position(8) == 3); // binary: 1000
}

} // anonymous namespace
