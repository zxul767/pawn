#include "Queen.h"

Queen::Queen ()
{
   rook = new Rook ();
   bishop = new Bishop ();
}

Queen::~Queen ()
{
   delete rook;
   delete bishop;
}

/*=============================================================================
  Get a bitboard containing all valid moves for a queen in LOCATION, assuming
  it is PLAYER's turn (moves that leave the king in check are also included)
  ===========================================================================*/
bitboard
Queen::get_moves (uint square, Player player, const Board* board) const
{
   bitboard attacks = bishop->get_moves (square, player, board);
   attacks |= rook->get_moves (square, player, board);

   return attacks;
}

bitboard
Queen::get_potential_moves (uint square, Player player) const
{
   bitboard moves = bishop->get_potential_moves (square, player);
   moves |= rook->get_potential_moves (square, player);

   return moves;
}
