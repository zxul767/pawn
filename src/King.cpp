#include "King.hpp"
#include <iostream>
#include <memory>

namespace game_rules
{
const bool King::neighbors_computed = King::compute_neighbors ();
bitboard King::neighbors[IBoard::SQUARES_COUNT];

King:: King ()
{
   compute_moves ();
}

King::~King () { }

/*=============================================================================
  Get all valid moves from SQUARE in the current BOARD, assumming it is
  PLAYER's turn to move (moves that leave the king in check are also included)
  ============================================================================*/
bitboard
King::get_moves (uint square, Player player, const IBoard* board) const
{
   if (!IBoard::is_inside_board (square))
      return 0;

   bitboard all_pieces = board->get_all_pieces ();
   bitboard attacks = get_potential_moves (square, player);

   // Include castling moves as valid
   if (IBoard::Squares (square) == board->get_initial_king_square (player))
   {
      // Ensure there are no pieces between the rook and the king
      if (board->can_castle (player, IBoard::KING_SIDE) &&
          !(all_pieces & (util::constants::ONE << (square + 1))) &&
          !(all_pieces & (util::constants::ONE << (square + 2))))
      {
         // Make sure there are no attacks on squares the king has to pass
         // through while castling
         if (!board->attacks_to (IBoard::Squares (square), false) &&
             !board->attacks_to (IBoard::Squares (square + 1), false) &&
             !board->attacks_to (IBoard::Squares (square + 2), false))
         {
            attacks |= (util::constants::ONE << (square + 2));
         }
      }

      if (board->can_castle (player, IBoard::QUEEN_SIDE) &&
          !(all_pieces & (util::constants::ONE << (square - 1))) &&
          !(all_pieces & (util::constants::ONE << (square - 2))) &&
          !(all_pieces & (util::constants::ONE << (square - 3))))
      {
         // Make sure there are no attacks on squares the king has to pass
         // through while castling
         if (!board->attacks_to (IBoard::Squares (square), false) &&
             !board->attacks_to (IBoard::Squares (square - 1), false) &&
             !board->attacks_to (IBoard::Squares (square - 2), false))
         {
            attacks |= (util::constants::ONE << (square - 2));
         }
      }
   }
   attacks &= ~board->get_pieces (player);

   return attacks;
}

bitboard
// Only for pawns is the player to move relevant in computing the potential moves
King:: get_potential_moves (uint square, Player /* player */) const
{
   if (IBoard::is_inside_board (square))
      return this->moves_from[square];

   return 0;
}


void
King::compute_moves ()
{
   int dx[KING_MOVES_COUNT] = {-1,  0, +1, +1, +1,  0, -1, -1};
   int dy[KING_MOVES_COUNT] = {+1, +1, +1,  0, -1, -1, -1,  0};

   for (uint square = 0; square < IBoard::SQUARES_COUNT; square++)
      this->moves_from[square] = 0;

   for (uint row = 0; row < IBoard::SIZE; ++row)
      for (uint col = 0; col < IBoard::SIZE; ++col)
      {
         /*------------------------------------------------------------
           Traverse all eight directions a king can move to:

           +------------------------+
           |    |    |    |    |    |
           --------------------------
           |    |  0 |  1 |  2 |    |
           --------------------------
           |    |  7 |  K |  3 |    |
           --------------------------
           |    |  6 |  5 |  4 |    |
           --------------------------
           |    |    |    |    |    |
           +------------------------+

           Jumps occur in clockwise order from 0 to 7.
           ------------------------------------------------------------*/

         uint square = row * IBoard::SIZE + col;

         for (uint jump = 0; jump < KING_MOVES_COUNT; jump++)
         {
            int y = row + dy[jump];
            int x = col + dx[jump];

            if (IBoard::is_inside_board (y, x))
               this->moves_from[square] |= (util::constants::ONE << (y * IBoard::SIZE + x));
         }
      }
}

bitboard
King::get_neighbors (uint position)
{
   if (IBoard::is_inside_board (position))
      return neighbors[position];
   return 0;
}

bool
King::compute_neighbors ()
{
   std::unique_ptr<Piece> king(new King());

   for (uint position = 0; position < IBoard::SQUARES_COUNT; ++position)
   {
      bitboard neighborhood = king->get_potential_moves (position, Piece::WHITE);
      bitboard actual_neighbors = neighborhood;

      int square = util::Util::MSB_position (neighborhood);

      while (neighborhood && square != -1)
      {
         actual_neighbors |= king->get_potential_moves (square, Piece::WHITE);
         neighborhood ^= (util::constants::ONE << square);
         square = util::Util::MSB_position (neighborhood);
      }

      neighbors[position] = actual_neighbors ^ (util::constants::ONE << position);
   }
   return true;
}

} // namespace game_rules
