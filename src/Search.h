#ifndef SEARCH_H
#define SEARCH_H

#include "Util.h"

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

   static const int DRAW_VALUE = 0;
   static const int MATE_VALUE = -Util::INFINITUM;
   static const uint MAX_QUIESCENCE_DEPTH = 4;

   Search () {}
   virtual ~Search () {}

   virtual void load_factor_weights (vector<int>& weights) = 0;

   virtual Result get_best_move (
       uint depth, Board* board, Move& best_move) = 0;

 protected:
   uint max_depth;
   uint n_nodes_evaluated;
   uint n_internal_nodes;
   uint n_leaf_nodes;
   uint average_branching_factor;
   int root_value;
};

#endif // SEARCH_H
