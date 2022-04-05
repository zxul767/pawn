#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>

#include "../../catch.hpp"
#include "bitboard.hpp"

namespace
{
using bits::BITS_IN_BITBOARD;
using bits::random_bitboard;
using std::cout;
using std::endl;

double benchmark_count_ones(
    std::function<uint(bits::bitboard)> count_ones, std::vector<bits::bitboard> values)
{
    const uint TRIALS = 10;
    clock_t start = clock();
    for (auto value : values)
    {
        for (uint i = 0; i < TRIALS; i++)
            REQUIRE(count_ones(value) != 0);
    }
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

std::vector<bits::bitboard> generate_bitboards()
{
    std::vector<bits::bitboard> result;
    const uint TRIALS = 50'000;
    for (uint trials = 0; trials < TRIALS; trials++)
    {
        result.push_back(random_bitboard(1 * BITS_IN_BITBOARD / 10));
        result.push_back(random_bitboard(3 * BITS_IN_BITBOARD / 10));
        result.push_back(random_bitboard(5 * BITS_IN_BITBOARD / 10));
        result.push_back(random_bitboard(7 * BITS_IN_BITBOARD / 10));
        result.push_back(random_bitboard(9 * BITS_IN_BITBOARD / 10));
    }
    return result;
}

void print_aligned(const std::string &prefix, double value)
{
    cout << std::setw(20) << std::left << prefix << value << endl;
}

TEST_CASE("utils::count_ones (perf)", "[bits][perf]")
{
    std::vector<bits::bitboard> tests = generate_bitboards();
    double sideways_time = benchmark_count_ones(&bits::count_ones, tests);
    double std_time = benchmark_count_ones(&bits::count_ones_std, tests);
    double baseline_time = benchmark_count_ones(&bits::count_ones_baseline, tests);
    double shift_time = benchmark_count_ones(&bits::count_ones_shift, tests);
    double adaptive_time = benchmark_count_ones(&bits::count_ones_adaptive, tests);

    cout << "+-----------------------------+" << endl;
    cout << "| utils::count_ones benchmark |" << endl;
    cout << "+-----------------------------+" << endl;

    print_aligned("sideways sum: ", sideways_time);
    print_aligned("std library: ", std_time);
    print_aligned("baseline: ", baseline_time);
    print_aligned("shift: ", shift_time);
    print_aligned("adaptive: ", adaptive_time);
    cout << endl;
    cout << "(all times in seconds)" << endl;
}

} // anonymous namespace
