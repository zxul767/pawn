#ifndef COMMON_H
#define COMMON_H

typedef unsigned int       u_int;
typedef unsigned long long bitboard;
typedef unsigned long long u_llong;
typedef unsigned short int u_short;

#include <climits>
#include <vector>

using std::vector;

class Util
{
 public:
   static const double      EPSILON = 1E-6;
   static const bitboard    one = 1;
   static const int         INFINITUM = (INT_MAX >> 1);

   static vector<bitboard>  to_bitboard;
   static bool              load_to_bitboard ();
   static bool              loaded;

   static int      MSB_position (bitboard bitvector);
   static int      LSB_position (bitboard bitvector);
   static u_int    count_set_bits (bitboard bitvector);
   static u_llong  rand64 ();
   static double   random (double low, double high);
   static void     to_binary (u_llong value);
   
   static int max (int a, int b)   { return (a > b ? a : b); }
   static u_short bit (u_short n)  { return 1 << n; }
};

#endif // COMMON_H
