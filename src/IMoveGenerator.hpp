#ifndef IMOVE_GENERATOR_H
#define IMOVE_GENERATOR_H

#include "Util.hpp"
#include <vector>

namespace game_rules { class IBoard; class Move; }

namespace game_engine
{
class IMoveGenerator
{
  public:
   enum Flags
   {
      SIMPLE            = 1 << 0,
      CAPTURES          = 1 << 1,
      CHECKS            = 1 << 2,
      CHECK_EVASIONS    = 1 << 3,
      PAWN_PROMOTIONS   = 1 << 4,
      ALL               = (SIMPLE | CAPTURES | CHECKS | CHECK_EVASIONS | PAWN_PROMOTIONS)
   };

   /*----------------------------------------------------------------------
     Return a vector of legal moves from the current BOARD configuration
     ---------------------------------------------------------------------*/
   virtual bool generate_moves (
       game_rules::IBoard*, std::vector<game_rules::Move>& moves, ushort flags) = 0;

   virtual bool generate_moves (game_rules::IBoard*, std::vector<game_rules::Move>& moves) = 0;
   virtual bool generate_en_prise_evations (game_rules::IBoard*, std::vector<game_rules::Move>& moves) = 0;

   virtual ~IMoveGenerator() {}
};

} // namespace game_engine

#endif // IMOVE_GENERATOR_H
