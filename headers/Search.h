#ifndef SEARCH_H
#define SEARCH_H

#include "common.h"

class Board;
class Move;

class Search
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

   static const int   DRAW_VALUE = 0;
   static const int   MATE_VALUE = -Util::INFINITUM;
   static const u_int MAX_QUIESCENCE_DEPTH = 4;

   Search () {}
   virtual ~Search () {}

   virtual void load_factor_weights (vector<int>& weights) = 0;

   virtual Result get_best_move (u_int  depth, 
                                 Board* board, 
                                 Move&  best_move) = 0;

 protected:
   u_int    max_depth;
   u_int    n_nodes_evaluated;
   u_int    n_internal_nodes;
   u_int    n_leaf_nodes;
   u_int    average_branching_factor;
   int      root_value;
};

#endif // SEARCH_H
