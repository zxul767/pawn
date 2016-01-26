#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"
#include "Board.h"

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

#endif
