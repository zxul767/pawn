#ifndef BISHOP_H
#define BISHOP_H

/*==============================================================================
  Encodes the move rules for a bishop, both in its general form (i.e. assumming
  an empty board), and in specific situations (i.e. in a board with pieces)
 ==============================================================================*/

#include "Piece.h"
#include "Board.h"

class Bishop : public Piece
{
public:
   Bishop ();
   ~Bishop ();

   bitboard get_moves (uint square, Player player, const Board* board) const;
   bitboard get_potential_moves (uint square, Player player) const;

private:
   bitboard get_ray (Board::Squares square, Diagonal direction) const;
   void compute_moves ();

   bitboard moves_from[Board::SQUARES][Piece::RAYS];
   bitboard all_moves_from[Board::SQUARES];
};

#endif
