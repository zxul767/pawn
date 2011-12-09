/*==============================================================================
 | Class name: Util                                                            |
 |                                                                             |
 | Responsabilities:                                                           |
 | -Provide several useful routines to manipulate bitboards and do operations  |
 |  with them.                                                                 |
 |                                                                             |
 | Date: September 1, 2007                                                     |
 ==============================================================================*/

#include "common.h"
#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

vector<bitboard>
Util::to_bitboard;

bool
Util::loaded = Util::load_to_bitboard ();

bool
Util::load_to_bitboard ()
{
   const static u_int SQUARES = 64;
   bitboard           one = 1;

   for (u_int i = 0; i < SQUARES; ++i)
      to_bitboard.push_back (one << i);
   
   return true;
}

/*==============================================================================
 | Get the position of the most significat bit in BITVECTOR (The algorithm used|
 | is based on binary search, see LMSBP_algorithm.txt for a description)       |
 |                                                                             |
 | Precondition: bitboard is a 64-bit integer type.                            |
 ==============================================================================*/
int
Util::MSB_position (bitboard bitvector)
{
   if (bitvector == 0) return -1;

   int position = 0;

   if (bitvector & (~0uLL << bit (5))) 
   { 
      position |= bit (5); 
      bitvector >>= bit (5); 
   }
   if (bitvector & (~0uLL << bit (4)))
   { 
      position |= bit (4); 
      bitvector >>= bit (4); 
   }
   if (bitvector & (~0uLL << bit (3))) 
   { 
      position |= bit (3); 
      bitvector >>= bit (3); 
   }
   if (bitvector & (~0uLL << bit (2))) 
   { 
      position |= bit (2); 
      bitvector >>= bit (2); 
   }
   if (bitvector & (~0uLL << bit (1))) 
   { 
      position |= bit (1); 
      bitvector >>= bit (1); 
   }
   if (bitvector & (~0uLL << bit (0))) 
   { 
      position |= bit (0);
   }

   return position;
}

/*==============================================================================
 | Get the position of the least significat bit in BITVECTOR (The algorithm    |
 | used is based on binary search, see LMSBP_algorithm.txt for a description)  |
 |                                                                             |
 | Precondition: bitboard is a 64-bit integer type.                            |
 ==============================================================================*/
int
Util::LSB_position (bitboard bitvector)
{
   if (bitvector == 0) return -1;

   int position = 0;

   if (!(bitvector & (~0uLL >> bit (5))))
   { 
      position |= bit (5);
      bitvector >>= bit (5);
   }
   if (!(bitvector & (~0uLL >> (bit (4) | bit (5)))))
   { 
      position |= bit (4);
      bitvector >>= bit (4);
   }
   if (!(bitvector & (~0uLL >> (bit (3) | bit (4) | bit (5)))))
   { 
      position |= bit (3);
      bitvector >>= bit (3);
   }
   if (!(bitvector & (~0uLL >> (bit (2) | bit (3) | bit (4) | bit (5)))))
   { 
      position |= bit (2); 
      bitvector >>= bit (2); 
   }
   if (!(bitvector & 
         (~0uLL >> (bit (1) | bit (2) | bit (3) | bit (4) | bit (5)))))
   { 
      position |= bit (1); 
      bitvector >>= bit (1); 
   }
   if (!(bitvector & 
         (~0uLL >> (bit (0) | bit (1) | bit (2) | bit (3) | bit (4) | bit (5)))))
   { 
      position |= bit (0);
   }

   return position;
}

/*==============================================================================
 | Complexity: O(n) where n is the number of turned-on bits                    |
 |                                                                             |
 | Note: There are at least two faster methods to do this task:                |
 |                                                                             |
 | (a) Using a precomputed table of size 2^8 to count the number of bits in    |
 | blocks of eight bits. The code required by this method is rather compact,   |
 | but requires a prior initialization. Complexity is O(1) both for space and  |
 | time.                                                                       |
 |                                                                             |
 | (b) Using binary counters of size 1, 2, 4, ..., 2^64. The code used by this |
 | method is rather compact, but without an explanation it is completely       |
 | cryptic. Complexity is O(1)                                                 |
 |                                                                             |
 | Precondition: bitboard is a 64-bit integer type.                            |
 ==============================================================================*/
u_int 
Util::count_set_bits (bitboard bitvector)
{
//    u_int n_bits = 0;

//    while (bitvector)
//    {
//       n_bits++;
//       bitvector &= bitvector - 1;
//    }
//    return n_bits;

// We are now using method (b)

   bitvector = (((0xAAAAAAAAAAAAAAAAuLL & bitvector) >> 0x1) + 
                ((0x5555555555555555uLL & bitvector)));

   bitvector = (((0xCCCCCCCCCCCCCCCCuLL & bitvector) >> 0x2) + 
                ((0x3333333333333333uLL & bitvector)));

   bitvector = (((0xF0F0F0F0F0F0F0F0uLL & bitvector) >> 0x4) + 
                ((0x0F0F0F0F0F0F0F0FuLL & bitvector)));

   bitvector = (((0xFF00FF00FF00FF00uLL & bitvector) >> 0x8) + 
                ((0x00FF00FF00FF00FFuLL & bitvector)));

   bitvector = (((0xFFFF0000FFFF0000uLL & bitvector) >> 0x10) + 
                ((0x0000FFFF0000FFFFuLL & bitvector)));

   bitvector = (((0xFFFFFFFF00000000uLL & bitvector) >> 0x20) + 
                ((0x00000000FFFFFFFFuLL & bitvector)));

   return (u_int) bitvector;
}

u_llong
Util::rand64 ()
{
   return (((u_llong) rand ()) ^ 
           ((u_llong) rand () << 15) ^ 
           ((u_llong) rand () << 30) ^ 
           ((u_llong) rand () << 45) ^ 
           ((u_llong) rand () << 60));
}

void
Util::to_binary (u_llong value)
{
   if (value > 0)
   {
      to_binary (value >> 1);
      cout << (value & 1 ? '1' : '0');
   }
}

double 
Util::random (double low, double high)
{
   double tmp;

   if (low > high)
   {
      tmp  = low;
      low  = high;
      high = tmp;
   }

   tmp = (rand () / static_cast<double> (RAND_MAX)) * (high-low) + low;
   return tmp;
}
