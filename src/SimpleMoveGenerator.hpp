#ifndef SIMPLE_MOVE_GENERATOR_H
#define SIMPLE_MOVE_GENERATOR_H

#include "MoveGenerator.hpp"

namespace game_engine
{
class SimpleMoveGenerator : public MoveGenerator
{
  public:
   SimpleMoveGenerator ();

   /*======================================================================
     Return a vector of legal moves from the current BOARD configuration
     =====================================================================*/
   bool generate_moves (game_rules::Board*, std::vector<game_rules::Move>& moves, ushort flags);
   bool generate_moves (game_rules::Board*, std::vector<game_rules::Move>& moves);
   bool generate_en_prise_evations (game_rules::Board*, std::vector<game_rules::Move>& moves);
};

} // namespace game_engine

#endif // SIMPLE_MOVE_GENERATOR_H
