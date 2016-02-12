#ifndef ALPHA_BETA_SEARCH_H
#define ALPHA_BETA_SEARCH_H

/*==============================================================================
  Implements the AI engine of the game (whose interface can be found in
  IChessEngine.h) using the alpha-beta search algorithm.
  ==============================================================================*/

#include "Util.hpp"
#include "IChessEngine.hpp"
#include "Move.hpp"

#include <stack>
#include <fstream>
#include <vector>

namespace game_engine
{
class PositionEvaluator;
class Dictionary;
class MoveGenerator;

class AlphaBetaSearch : public IChessEngine
{
  private:
   int alpha_beta (uint depth, int alpha, int beta);
   int quiescence (uint depth, int alpha, int beta);
   int iterative_deepening (std::vector<game_rules::Move>& principal_variation);

   void print_statistics (std::vector<game_rules::Move>& principal_variation);
   void reset_statistics ();

   bool build_principal_variation (game_rules::Board*, std::vector<game_rules::Move>& principal_variation);
   void load_factor_weights (std::vector<int>& weights);

   PositionEvaluator* evaluator;
   MoveGenerator* generator;
   Dictionary* hash_table;
   game_rules::Board* board;

   Result result;
   uint hash_table_hits;
   game_rules::Move best_move;

  public:
   AlphaBetaSearch (PositionEvaluator*, MoveGenerator*);
   ~AlphaBetaSearch ();

   Result get_best_move (uint depth, game_rules::Board*, game_rules::Move& best_move);
};

} // namespace game_engine

#endif // ALPHA_BETA_SEARCH_H
