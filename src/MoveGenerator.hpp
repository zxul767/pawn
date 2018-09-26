#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "IMoveGenerator.hpp"

namespace game_engine
{
class MoveGenerator : public IMoveGenerator
{
  public:
   /*======================================================================
     Return a vector of legal moves from the current BOARD configuration
     =====================================================================*/
   bool generate_moves (game_rules::IBoard*, std::vector<game_rules::Move>& moves, ushort kind_of_moves);
   bool generate_moves (game_rules::IBoard*, std::vector<game_rules::Move>& moves);
   bool generate_en_prise_evations (game_rules::IBoard*, std::vector<game_rules::Move>& moves);

   ~MoveGenerator() {}
};

} // namespace game_engine

#endif // MOVE_GENERATOR_H
