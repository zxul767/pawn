#ifndef PIECE_H
#define PIECE_H

/*==============================================================================
  Base class for different pieces of the game, each one responsible for their
  own move rules
 ==============================================================================*/

#include "Util.hpp"
#include <string>

// Cannot include Board.h because that creates a cyclic dependency
class Board;

class Piece
{
public:
   Piece () { }
   virtual ~Piece () { }

   static const uint PIECES = 6;

   // Number of directions a bishop or rook can move to.
   static const uint RAYS = 4;

   // Number of jumps a knight can make in the center of the board.
   static const uint KNIGHT_MOVES = 8;

   // Number of moves a king can make in the center of the board.
   static const uint KING_MOVES = 8;

   // Number of moves a pawn can make in the center of the board.
   static const uint PAWN_MOVES = 3;

   enum Type {
      PAWN,
      KNIGHT,
      BISHOP,
      ROOK,
      QUEEN,
      KING,
      NULL_PIECE
   };

   enum Player {
      WHITE,
      BLACK,
      NULL_PLAYER
   };

   enum Diagonal {
      NORTH_EAST,
      SOUTH_EAST,
      SOUTH_WEST,
      NORTH_WEST,
      NULL_DIAGONAL
   };

   enum RowColumn {
      NORTH,
      EAST,
      SOUTH,
      WEST,
      NULL_LINE
   };

   static std::string pieceString (Type piece_type);

   virtual bitboard get_moves (
       uint square, Player player, const Board* board) const = 0;

   virtual bitboard get_potential_moves (uint  square, Player player) const = 0;
};

inline Piece::Diagonal&
operator ++ (Piece::Diagonal& direction)
{
   if (direction == Piece::NULL_DIAGONAL)
      return (direction = Piece::NULL_DIAGONAL);

   return (direction = Piece::Diagonal (direction + 1));
}

inline Piece::RowColumn&
operator ++ (Piece::RowColumn& direction)
{
   if (direction == Piece::NULL_LINE)
      return (direction = Piece::NULL_LINE);

   return (direction = Piece::RowColumn (direction + 1));
}

inline Piece::Type&
operator ++ (Piece::Type& piece_type)
{
   if (piece_type == Piece::NULL_PIECE)
      return (piece_type = Piece::NULL_PIECE);

   return (piece_type = Piece::Type (piece_type + 1));
}

inline Piece::Type&
operator -- (Piece::Type& piece_type)
{
   if (piece_type == Piece::PAWN)
      return (piece_type = Piece::NULL_PIECE);

   return (piece_type = Piece::Type (piece_type - 1));
}

inline Piece::Player&
operator ++ (Piece::Player& player)
{
   if (player == Piece::NULL_PLAYER)
      return (player = Piece::NULL_PLAYER);

   return (player = Piece::Player (player + 1));
}

#endif // PIECE_H
