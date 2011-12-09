#ifndef ALPHA_BETA_SEARCH_H
#define ALPHA_BETA_SEARCH_H

#include "common.h"
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
   int  alpha_beta (u_int depth, int alpha, int beta);
   int  quiescence (u_int depth, int alpha, int beta);
   int  iterative_deepening (vector<Move>& PV);

   void print_statistics (vector<Move>& PV);
   void reset_statistics ();
   bool build_PV (Board* board, vector<Move>& PV);
   void load_factor_weights (vector<int>& weights);

   PositionEvaluator* evaluator;
   MoveGenerator*     generator;
   Dictionary*        hash_table;
   Board*             board;

   Result result;
   u_int  hash_table_hits;
   Move   best_move;

   bool debugging;
   void turn_debugging (bool value) { debugging = value; }

 public:
   AlphaBetaSearch (PositionEvaluator* evaluator, MoveGenerator* generator);
   ~AlphaBetaSearch ();
   
   Result get_best_move (u_int depth, Board* board, Move& best_move);
};

#endif // ALPHA_BETA_SEARCH_H
