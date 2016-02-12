#ifndef ICHESS_ENGINE_H
#define ICHESS_ENGINE_H

#include "Util.hpp"

namespace game_rules { class Board; class Move; }

namespace game_engine
{
using util::uint;

class IChessEngine
{
  public:

   enum Result {
      BLACK_MATES = -util::constants::INFINITUM,
      WHITE_MATES = +util::constants::INFINITUM,
      STALEMATE,
      DRAW_BY_REPETITION,
      NORMAL_EVALUATION,
      ERROR
   };

   static const int DRAW_VALUE = 0;
   static const int MATE_VALUE = -util::constants::INFINITUM;
   static const uint MAX_QUIESCENCE_DEPTH = 4;

   IChessEngine () {}
   virtual ~IChessEngine () {}

   virtual void load_factor_weights (std::vector<int>& weights) = 0;
   virtual Result get_best_move (uint depth, game_rules::Board*, game_rules::Move& best_move) = 0;

  protected:
   uint max_depth;
   uint n_nodes_evaluated;
   uint n_internal_nodes;
   uint n_leaf_nodes;
   uint average_branching_factor;
   int root_value;
};

} // namespace game_engine

#endif // ICHESS_ENGINE_H
