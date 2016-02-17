#ifndef BOARD_STATUS_H
#define BOARD_STATUS_H

/*==============================================================================
  Represents the set of traits of a chess board configuration that allows to
  correctly take back (i.e. unmake) moves at any point during the game.
  ==============================================================================*/

#include "Move.hpp"

namespace game_rules
{
struct BoardConfiguration
{
   BoardConfiguration (const Move& move,
                bitboard en_passant,
                bool can_castle_king_side,
                bool can_castle_queen_side,
                ullong hash_key,
                ullong hash_lock)
   {
      this->move = move;
      this->en_passant_capture_square = en_passant;
      this->can_castle_king_side = can_castle_king_side;
      this->can_castle_queen_side = can_castle_queen_side;
      this->hash_key = hash_key;
      this->hash_lock = hash_lock;
   }

   Move move;
   bitboard en_passant_capture_square;
   bool can_castle_king_side;
   bool can_castle_queen_side;
   ullong hash_key;
   ullong hash_lock;
};

} // namespace game_rules

#endif // BOARD_STATUS_H
