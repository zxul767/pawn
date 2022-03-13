#ifndef ICHESS_ENGINE_H
#define ICHESS_ENGINE_H

#include "Util.hpp"
#include "SearchStats.hpp"

namespace game_rules { class IBoard; class Move; }

namespace game_engine
{
class IEngine
{
  public:

   enum GameResult {
      BLACK_MATES = -util::constants::INFINITUM,
      WHITE_MATES = +util::constants::INFINITUM,
      STALEMATE,
      DRAW_BY_REPETITION,
      NORMAL_EVALUATION,
      ERROR
   };

   static const int DRAW_VALUE = 0;
   static const int MATE_VALUE = -util::constants::INFINITUM;
   static const uint MAX_QUIESCENCE_DEPTH = 10;

   IEngine () {}
   virtual ~IEngine () {}

   virtual void load_factor_weights (std::vector<int>& weights) = 0;
   virtual GameResult get_best_move (uint max_depth, game_rules::IBoard*, game_rules::Move& best_move) = 0;

   SearchStats statistics;

  protected:
   uint max_depth;
   int root_value;
};

} // namespace game_engine

#endif // ICHESS_ENGINE_H
