#ifndef MINIMAX_SEARCH_H
#define MINIMAX_SEARCH_H

#include "Util.h"
#include "Search.h"
#include "Move.h"

#include <vector>

class MoveGenerator;
class PositionEvaluator;

class MinimaxSearch : public Search
{
 private:
   int minimax (uint depth, Result& result);

   MoveGenerator* generator;
   PositionEvaluator* evaluator;
   Board* board;

   Move best_move;
   Result result;

 public:
   MinimaxSearch (PositionEvaluator* evaluator, MoveGenerator* generator);
   ~MinimaxSearch ();

   Result get_best_move (uint max_depth, Board* board, Move& best_move);
   void load_factor_weights (vector<int>& weights);
};

#endif // MINIMAX_SEARCH
