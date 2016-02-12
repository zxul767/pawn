#ifndef KNIGHT_H
#define KNIGHT_H

/*==============================================================================
  Encodes the move rules for a knight, both in its general form (i.e. assumming
  an empty board), and in specific situations (i.e. in a board with pieces)
  ==============================================================================*/

#include "Piece.hpp"
#include "Board.hpp"

namespace game_rules
{
class Knight : public Piece
{
  public:
   Knight ();
   ~Knight ();

   bitboard get_moves (uint square, Player player, const Board* board) const;
   bitboard get_potential_moves (uint square, Player player) const;

  private:
   void compute_moves ();

   bitboard moves_from[Board::SQUARES];
};

} // namespace game_rules

#endif // KNIGHT_H
