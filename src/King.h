#ifndef KING_H
#define KING_H

#include "Piece.h"
#include "Board.h"

class King : public Piece
{
public:
   King ();
   ~King ();

   bitboard get_moves (uint square, Player player, const Board* board) const;
   bitboard get_potential_moves (uint square, Player player) const;

   static bitboard get_neighbors (uint position);

private:
   void compute_moves ();
   bitboard moves_from[Board::SQUARES];

   static bitboard neighbors[Board::SQUARES];
   static bool compute_neighbors ();
   const static bool neighbors_computed;
};

#endif
