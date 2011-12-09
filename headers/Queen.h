/*=============================================================================
  Class name: Bishop

  Responsabilities: 
  -Provide the set of moves a queen can make from any square on the board.

  Collaborations:

  Version: 0.1

  Date: September 01, 2007
  =============================================================================*/
#ifndef QUEEN_H
#define QUEEN_H

#include "Rook.h"
#include "Bishop.h"

class Queen : public Piece
{
 public:
   Queen ();
   ~Queen ();

   bitboard get_moves (u_int square, Player player, const Board* board) const;

   bitboard get_potential_moves (u_int square, Player player) const;

 private:
   // Queen's moves are simply the combination of Rook and Bishop's moves
   Rook*   rook;
   Bishop* bishop;
};

#endif
