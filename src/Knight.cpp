/*==============================================================================
  Encodes the move rules for a knight, both in its general form (i.e. assumming
  an empty board), and in specific situations (i.e. in a board with pieces)
  ==============================================================================*/

#include "Knight.hpp"
#include "IBoard.hpp"

namespace game_rules
{
Knight::Knight ()
{
   compute_moves ();
}

Knight::~Knight () { }

/*=============================================================================
  Get all valid moves from SQUARE in the current BOARD, assumming it is
  PLAYER's turn to move (moves that leave the king in check are also included)
  ============================================================================*/
bitboard
Knight::get_moves (uint square, Player player, const IBoard* board) const
{
   bitboard attacks = get_potential_moves (square, player);
   attacks &= ~board->get_pieces (player);

   return attacks;
}

/*=============================================================================
  Return all possible moves from SQUARE, assuming the board is empty.
  ============================================================================*/
bitboard
// Only for pawns is the player to move relevant in computing the potential moves
Knight::get_potential_moves (uint square, Player /* player */) const
{
   if (IBoard::is_inside_board (square))
      return this->moves_from[square];

   return 0;
}

/*=============================================================================
  Compute all moves a bishop can make from every square on the board assuming
  the board is empty.
  ============================================================================*/
void
Knight::compute_moves ()
{
   int dx[KNIGHT_MOVES_COUNT] = {+1, +2, +2, +1, -1, -2, -2, -1};
   int dy[KNIGHT_MOVES_COUNT] = {-2, -1, +1, +2, +2, +1, -1, -2};

   for (uint square = 0; square < BOARD_SQUARES_COUNT; ++square)
      this->moves_from[square] = 0;

   for (uint row = 0; row < BOARD_SIZE; ++row)
      for (uint col = 0; col < BOARD_SIZE; ++col)
      {
         /*--------------------------------------------------------------------
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

           -------------------------------------------------------------------*/
         uint square = row * BOARD_SIZE + col;
         for (uint jump = 0; jump < KNIGHT_MOVES_COUNT; ++jump)
         {
            int y = row + dy[jump];
            int x = col + dx[jump];
            if (IBoard::is_inside_board (y, x))
               this->moves_from[square] |= (util::constants::ONE << (y * BOARD_SIZE + x));
         }
      }
}

} // namespace game_rules
