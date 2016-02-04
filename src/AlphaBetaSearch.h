#ifndef ALPHA_BETA_SEARCH_H
#define ALPHA_BETA_SEARCH_H

#include "Util.h"
#include "Search.h"
#include "Move.h"

#include <stack>
#include <fstream>
#include <vector>

using std::vector;

class MoveGenerator;
class PositionEvaluator;
class Dictionary;

class AlphaBetaSearch : public Search
{
 private:
   int alpha_beta (uint depth, int alpha, int beta);
   int quiescence (uint depth, int alpha, int beta);
   int iterative_deepening (vector<Move>& principal_variation);

   void print_statistics (vector<Move>& principal_variation);
   void reset_statistics ();
   bool build_principal_variation (Board* board, vector<Move>& principal_variation);
   void load_factor_weights (vector<int>& weights);

   PositionEvaluator* evaluator;
   MoveGenerator* generator;
   Dictionary* hash_table;
   Board* board;

   Result result;
   uint hash_table_hits;
   Move best_move;

 public:
   AlphaBetaSearch (PositionEvaluator* evaluator, MoveGenerator* generator);
   ~AlphaBetaSearch ();

   Result get_best_move (uint depth, Board* board, Move& best_move);
};

#endif // ALPHA_BETA_SEARCH_H
