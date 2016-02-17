#ifndef ALPHA_BETA_SEARCH_H
#define ALPHA_BETA_SEARCH_H

/*==============================================================================
  Implements the AI engine of the game (whose interface can be found in IEngine.h)
  using the alpha-beta search algorithm.
  ==============================================================================*/

#include "Util.hpp"
#include "IEngine.hpp"
#include "Move.hpp"

#include <stack>
#include <fstream>
#include <vector>

namespace game_engine
{
class IPositionEvaluator;
class TranspositionTable;
class MoveGenerator;

class AlphaBetaSearch : public IEngine
{
  private:
   int alpha_beta (uint depth, int alpha, int beta);
   int quiescence (uint depth, int alpha, int beta);
   int iterative_deepening (std::vector<game_rules::Move>& principal_variation);

   void print_statistics (std::vector<game_rules::Move>& principal_variation);
   void reset_statistics ();

   bool build_principal_variation (game_rules::IBoard*, std::vector<game_rules::Move>& principal_variation);
   void load_factor_weights (std::vector<int>& weights);

   IPositionEvaluator* position_evaluator;
   MoveGenerator* move_generator;
   TranspositionTable* transposition_table;
   game_rules::IBoard* board;

   GameResult result;
   uint hash_table_hits;
   game_rules::Move best_move;

  public:
   AlphaBetaSearch (IPositionEvaluator*, MoveGenerator*);
   ~AlphaBetaSearch ();

   GameResult get_best_move (uint depth, game_rules::IBoard*, game_rules::Move& best_move);
};

} // namespace game_engine

#endif // ALPHA_BETA_SEARCH_H
