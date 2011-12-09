#ifndef SIMPLE_MOVE_GENERATOR_H
#define SIMPLE_MOVE_GENERATOR_H

#include "MoveGenerator.h"

class SimpleMoveGenerator : public MoveGenerator
{
 public:
   SimpleMoveGenerator ();

   /*====================================================================== 
     Return a vector of legal moves from the current BOARD configuration
     =====================================================================*/
   bool generate_moves (Board* board, vector<Move>& moves, u_short flags);
   bool generate_moves (Board* board, vector<Move>& moves);
   bool generate_en_prise_evations (Board* board, vector<Move>& moves);
};

#endif // SIMPLE_MOVE_GENERATOR_H
