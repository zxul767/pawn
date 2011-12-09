#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <vector>
#include "Move.h"
class Board;

using std::vector;

class MoveGenerator
{
 public:
   MoveGenerator () {}
   virtual ~MoveGenerator () {}

   enum Flags
   {
      SIMPLE            = 1 << 0,
      CAPTURES          = 1 << 1,
      CHECKS            = 1 << 2,
      CHECK_EVASIONS    = 1 << 3,
      PAWN_PROMOTIONS   = 1 << 4,
      ALL               = (SIMPLE | CAPTURES | CHECKS | CHECK_EVASIONS | 
                           PAWN_PROMOTIONS)
   };

   /*====================================================================== 
     Return a vector of legal moves from the current BOARD configuration
     =====================================================================*/
   virtual bool generate_moves (Board*  board, vector<Move>& moves, 
                                u_short flags) = 0;

   virtual bool generate_moves (Board* board, vector<Move>& moves) = 0;
   virtual bool generate_en_prise_evations (Board* board, vector<Move>& moves) = 0;
};

#endif // MOVE_GENERATOR_H
