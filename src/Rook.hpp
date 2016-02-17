#ifndef ROOK_H
#define ROOK_H

/*==============================================================================
  Encodes the move rules for a rook, both in its general form (i.e. assumming
  an empty board), and in specific situations (i.e. in a board with pieces)
 ==============================================================================*/

#include "Piece.hpp"
#include "IBoard.hpp"

namespace game_rules
{
class Rook : public Piece
{
public:
   Rook ();
   ~Rook ();

   bitboard get_moves (uint square, Player player, const IBoard* board) const;
   bitboard get_potential_moves (uint square, Player player) const;

private:
   bitboard get_ray_from (IBoard::Squares square, RowColumn direction) const;
   void compute_moves ();

   bitboard moves_from[IBoard::SQUARES_COUNT][Piece::RAY_DIRECTIONS_COUNT];
   bitboard all_moves_from[IBoard::SQUARES_COUNT];
};

} // namespace game_rules

#endif // ROOK_H
