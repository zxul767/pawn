/*=============================================================================
  Class name: Bishop

  Responsabilities: 
  -Provide the set of moves a bishop can make from any square on the board.

  Date: September 01, 2007
  ===========================================================================*/

#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"
#include "Board.h"

class Bishop : public Piece
{
 public:
   Bishop ();
   ~Bishop ();

   bitboard get_moves (u_int square, 
                       Player player, 
                       const Board* board) const;

   bitboard get_potential_moves (u_int square, Player player) const;

 private:

   bitboard get_ray (Board::Squares square, Diagonal direction) const;
   void     compute_moves ();

   bitboard moves_from[Board::SQUARES][Piece::RAYS];
   bitboard all_moves_from[Board::SQUARES];
};

#endif
