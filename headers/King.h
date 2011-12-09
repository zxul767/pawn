/*=============================================================================
  Class name: King

  Responsabilities: 
  -Provide the set of moves a king can make from any square on the board.

  Date: September 01, 2007
  ===========================================================================*/

#ifndef KING_H
#define KING_H

#include "Piece.h"
#include "Board.h"

class King : public Piece
{
 public:
   King ();
   ~King ();
                
   bitboard get_moves (u_int square, 
                       Player player, 
                       const Board* board) const;

   bitboard get_potential_moves (u_int square, Player player) const;

   static bitboard get_neighbors (u_int position);

 private:
   void compute_moves ();
                
   bitboard moves_from[Board::SQUARES];
   static bitboard neighbors[Board::SQUARES];
   static bool compute_neighbors ();
   const static bool neighbors_computed;
};


#endif
