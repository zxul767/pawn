#ifndef ROOK_H
#define ROOK_H

/*==============================================================================
  Encodes the move rules for a rook, both in its general form (i.e. assumming
  an empty board), and in specific situations (i.e. in a board with pieces)
 ==============================================================================*/

#include "Piece.hpp"
#include "Board.hpp"

class Rook : public Piece
{
public:
   Rook ();
   ~Rook ();

   bitboard get_moves (uint square, Player player, const Board* board) const;
   bitboard get_potential_moves (uint square, Player player) const;

private:
   bitboard get_ray (Board::Squares square, RowColumn direction) const;
   void compute_moves ();

   bitboard moves_from[Board::SQUARES][Piece::RAYS];
   bitboard all_moves_from[Board::SQUARES];
};

#endif
