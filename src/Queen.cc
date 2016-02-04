#include "Queen.h"

Queen::Queen ()
{
   this->rook = new Rook ();
   this->bishop = new Bishop ();
}

Queen::~Queen ()
{
   delete this->rook;
   delete this->bishop;
}

/*=============================================================================
  Get a bitboard containing all valid moves for a queen in LOCATION, assuming
  it is PLAYER's turn (moves that leave the king in check are also included)
  ===========================================================================*/
bitboard
Queen::get_moves (uint square, Player player, const Board* board) const
{
   bitboard attacks = this->bishop->get_moves (square, player, board);
   attacks |= this->rook->get_moves (square, player, board);

   return attacks;
}

bitboard
Queen::get_potential_moves (uint square, Player player) const
{
   bitboard moves = this->bishop->get_potential_moves (square, player);
   moves |= this->rook->get_potential_moves (square, player);

   return moves;
}
