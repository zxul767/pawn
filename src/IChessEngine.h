#ifndef ICHESS_ENGINE_H
#define ICHESS_ENGINE_H

#include "Util.h"

class Board;
class Move;

class IChessEngine
{
   public:

   enum Result {
      BLACK_MATES = -Util::INFINITUM,
      WHITE_MATES = +Util::INFINITUM,
      STALEMATE,
      DRAW_BY_REPETITION,
      NORMAL_EVALUATION,
      ERROR
   };

   static const int DRAW_VALUE = 0;
   static const int MATE_VALUE = -Util::INFINITUM;
   static const uint MAX_QUIESCENCE_DEPTH = 4;

   IChessEngine () {}
   virtual ~IChessEngine () {}

   virtual void load_factor_weights (std::vector<int>& weights) = 0;
   virtual Result get_best_move (uint depth, Board* board, Move& best_move) = 0;

 protected:
   uint max_depth;
   uint n_nodes_evaluated;
   uint n_internal_nodes;
   uint n_leaf_nodes;
   uint average_branching_factor;
   int root_value;
};

#endif // ICHESS_ENGINE_H