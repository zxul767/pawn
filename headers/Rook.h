/*=============================================================================
  Class name: Rook

  Responsabilities:
  -Provide the set of moves a bishop can make from any square on the board.
  =============================================================================*/

#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"
#include "Board.h"

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
