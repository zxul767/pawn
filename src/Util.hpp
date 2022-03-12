#ifndef UTIL_H
#define UTIL_H

#include <climits>
#include <vector>
#include <cstddef>

typedef unsigned short int ushort;
typedef unsigned int uint;
typedef unsigned long long ullong;

namespace util
{
typedef unsigned long long bitboard;

namespace constants
{
constexpr int INFINITUM = INT_MAX / 2;
constexpr double EPSILON = 1E-6;
constexpr bitboard ONE = 1;
}

class Util
{
  public:
   static std::vector<bitboard> to_bitboard;
   static bool load_to_bitboard ();
   static bool loaded;

   static int MSB_position (bitboard bitvector);
   static int LSB_position (bitboard bitvector);
   static uint count_set_bits (bitboard bitvector);
   static ullong random_ullong ();
   static double random (double low, double high);
   static void to_binary (ullong value);

   static int max (int a, int b) { return (a > b ? a : b); }
   static ushort bit (ushort n) { return 1 << n; }
};

bool is_odd(uint n);

} // namespace util

#endif // UTIL_H
