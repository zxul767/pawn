/*=============================================================================
  Class name: Knight

  Responsabilities: 
  -Provide the set of moves a knight can make from any square on the board.

  Collaborations:

  Version: 0.1

  Date: September 01, 2007
  ============================================================================*/

#include "Knight.h"

Knight::Knight ()
{
   compute_moves ();
}

Knight::~Knight ()
{
}

/*=============================================================================
  Get all valid moves from SQUARE in the current BOARD, assumming it is 
  PLAYER's turn to move (moves that leave the king in check are also included)
  ============================================================================*/
bitboard
Knight::get_moves (uint          square, 
                   Player         player, 
                   const Board*   board) const
{
   bitboard attacks = get_potential_moves (square, player);
   attacks &= ~board->get_pieces (player);

   return attacks;
}

/*=============================================================================
  Return all possible moves from SQUARE, assuming the board is empty.
  ============================================================================*/
bitboard 
Knight::get_potential_moves (uint square, Player player) const
{
   if (Board::is_inside_board (square))
      return moves_from[square];

   return 0;
}

/*=============================================================================
  Compute all moves a bishop can make from every square on the board assuming
  the board is empty.
  ============================================================================*/
void
Knight::compute_moves ()
{
   int dx[KNIGHT_MOVES] = {+1, +2, +2, +1, -1, -2, -2, -1};
   int dy[KNIGHT_MOVES] = {-2, -1, +1, +2, +2, +1, -1, -2};

   for (uint square = 0; square < Board::SQUARES; ++square)
      moves_from[square] = 0;

   for (uint row = 0; row < Board::SIZE; ++row)
      for (uint col = 0; col < Board::SIZE; ++col)
      {
         /*====================================================================
           Traverse all eight directions a knight can move to:
           
           +------------------------+
           |    | 7  |    | 0  |    |    Jumps occur in clockwise order 
           --------------------------    from 0 to 7.
           | 6  |    |    |    | 1  |
           --------------------------
           |    |    | KN |    |    |
           --------------------------
           | 5  |    |    |    | 2  |
           --------------------------
           |    | 4  |    | 3  |    |
           +------------------------+

           ===================================================================*/
         uint square = row * Board::SIZE + col;
         for (uint jump = 0; jump < KNIGHT_MOVES; ++jump)
         {
            int y = row + dy[jump];
            int x = col + dx[jump];
            if (Board::is_inside_board (y, x))
               moves_from[square] |= (Util::one << (y * Board::SIZE + x));
         }
      }
}

