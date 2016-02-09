#ifndef UTIL_H
#define UTIL_H

typedef unsigned int uint;
typedef unsigned long long bitboard;
typedef unsigned long long ullong;
typedef unsigned short int ushort;

#include <climits>
#include <vector>

class Util
{
 public:
   static constexpr double EPSILON = 1E-6;
   static const bitboard one = 1;
   static const int INFINITUM = (INT_MAX >> 1);

   static std::vector<bitboard> to_bitboard;
   static bool load_to_bitboard ();
   static bool loaded;

   static int MSB_position (bitboard bitvector);
   static int LSB_position (bitboard bitvector);
   static uint count_set_bits (bitboard bitvector);
   static ullong rand64 ();
   static double random (double low, double high);
   static void to_binary (ullong value);

   static int max (int a, int b) { return (a > b ? a : b); }
   static ushort bit (ushort n) { return 1 << n; }
};

#endif // UTIL_H
