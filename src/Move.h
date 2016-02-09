/*=============================================================================
  Class name: Move

  Responsabilities:
  -Specify the initial and final coordinates of a moving piece
  -Specify the nature of the move (CAPTURE, PAWN PROMOTION, etc.)
  ===========================================================================*/
#ifndef MOVE_H
#define MOVE_H

#include <string>
#include "Piece.h"
#include "Board.h"

class Move
{
 public:
   Move ();
   Move (const std::string& notation);
   Move (Board::Squares start, Board::Squares end);
   Move (const Move& move);

   /*--------------------------------------------------------------------------
     PUBLIC CONSTANTS
     ------------------------------------------------------------------------*/
   enum Type {
      SIMPLE_MOVE,
      NORMAL_CAPTURE,
      CHECK,
      CASTLE_KING_SIDE,
      CASTLE_QUEEN_SIDE,
      PROMOTION_MOVE,
      EN_PASSANT_CAPTURE,
      NULL_MOVE
   };

   /*--------------------------------------------------------------------------
     PUBLIC METHODS
     ------------------------------------------------------------------------*/
   Board::Squares from () const;
   Board::Squares to () const;

   Type get_type () const;
   void set_type (Type type);

   Piece::Type get_moving_piece () const;
   Piece::Type get_captured_piece () const;

   void set_moving_piece (Piece::Type piece);
   void set_captured_piece (Piece::Type piece);

   void set_score (int score);
   int  get_score () const;

   bool is_null () const;

   /*--------------------------------------------------------------------------
     OVERLOADED OPERATORS
     ------------------------------------------------------------------------*/
   friend std::ostream& operator << (std::ostream& out, const Move& move);

   friend bool operator == (const Move& m1, const Move& m2);
   friend bool operator < (const Move& m1, const Move& m2);

   /*--------------------------------------------------------------------------
     PUBLIC STATIC METHODS
     ------------------------------------------------------------------------*/
   static bool translate_to_square (const std::string& notation, Board::Squares& square);
   static bool translate_to_notation (Board::Squares square, std::string& notation);
   static bool is_valid_notation (const std::string &notation);

 private:
   Board::Squares start;
   Board::Squares end;
   Type type;
   Piece::Type moving_piece;
   Piece::Type captured_piece;

   // The score assigned to this move is relative to the position of the board
   int score;
};

#endif // MOVE_H
