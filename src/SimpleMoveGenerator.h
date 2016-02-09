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
   bool generate_moves (Board* board, std::vector<Move>& moves, ushort flags);
   bool generate_moves (Board* board, std::vector<Move>& moves);
   bool generate_en_prise_evations (Board* board, std::vector<Move>& moves);
};

#endif // SIMPLE_MOVE_GENERATOR_H
