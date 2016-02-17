/*==============================================================================
  Defines a few common operations for the base class. See IBoard.h to see the
  interface specification.
  ==============================================================================*/

#include "IBoard.hpp"

namespace game_rules
{
IBoard::IBoard ()
{
}

IBoard::~IBoard ()
{
}

bool
IBoard::is_inside_board (int row, int col)
{
   return (row >= 0 && row < (int)SIZE) && (col >= 0 && col < (int)SIZE);
}

bool
IBoard::is_inside_board (uint row, uint col)
{
   return (row < SIZE) && (col < SIZE);
}

bool
IBoard::is_inside_board (uint square)
{
   return (square < SQUARES_COUNT);
}

std::ostream&
operator << (std::ostream& out, const IBoard& board)
{
   static const std::string
         piece_to_string[Piece::PIECES_COUNT][IBoard::PLAYERS_COUNT] =
         {
            { "  P  ", " *P* " },
            { "  N  ", " *N* " },
            { "  B  ", " *B* " },
            { "  R  ", " *R* " },
            { "  Q  ", " *Q* " },
            { "  K  ", " *K* " },
         };

   out << "  -------------------------------------------------" << std::endl;
   for (uint row = 0; row < IBoard::SIZE; ++row)
   {
      out << "  |     |     |     |     |     |     |     |     |" << std::endl;
      out << (IBoard::SIZE - row) << ' ';

      for (uint col = 0; col < IBoard::SIZE; ++col)
      {
         out << '|';

         IBoard::Squares square = (IBoard::Squares) (row * IBoard::SIZE + col);
         Piece::Player  player = board.get_piece_color (square);
         Piece::Type    piece  = board.get_piece (square);

         if ((player != Piece::NULL_PLAYER) && (piece != Piece::NULL_PIECE))
         {
            out << piece_to_string[piece][player];
         }
         else
            // If there is nothing in (row, col) just output an empty square
            out << "     ";

      }
      out << '|' << std::endl;
      out << "  -------------------------------------------------" << std::endl;
   }

   // Display algebraic coordinates
   out << "     a     b     c     d     e     f     g     h   " << std::endl;

   return out;
}

} // namespace game_rules
