#ifndef BOARD_H
#define BOARD_H

/*==============================================================================
  Represents a non-static chess board configuration (i.e. where the pieces are,
  whether there is an en-passant capture possibility, what castling privileges
  remain, etc.) that also allows making and taking back moves, according to the
  usual rules of chess.
  ==============================================================================*/

#include <string>
#include <iostream>

#include "Util.hpp"
#include "Piece.hpp"

namespace game_rules
{
class Move;

class Board
{
  public:
   Board ();
   virtual ~Board ();

   static constexpr uint SQUARES = 64;
   static constexpr uint SIZE = 8;
   static constexpr uint PLAYERS = 2;

   enum Squares {
      a8, b8, c8, d8, e8, f8, g8, h8,
      a7, b7, c7, d7, e7, f7, g7, h7,
      a6, b6, c6, d6, e6, f6, g6, h6,
      a5, b5, c5, d5, e5, f5, g5, h5,
      a4, b4, c4, d4, e4, f4, g4, h4,
      a3, b3, c3, d3, e3, f3, g3, h3,
      a2, b2, c2, d2, e2, f2, g2, h2,
      a1, b1, c1, d1, e1, f1, g1, h1
   };

   enum CastleSide {
      KING_SIDE, QUEEN_SIDE
   };

   enum GameStatus {
      GAME_OVER, PENDING_GAME
   };

   enum Error {
      KING_LEFT_IN_CHECK,
      OPPONENTS_TURN,
      WRONG_MOVEMENT,
      GAME_FINISHED,
      NO_PIECE_IN_SQUARE,
      DRAW_BY_REPETITION,
      NO_ERROR
   };

   virtual void clear () = 0;
   virtual void reset () = 0;

   virtual bool load_game (const std::string& file) = 0;
   virtual bool save_game (const std::string& file) = 0;

   virtual bool add_piece (
       const std::string& location, Piece::Type piece, Piece::Player player) = 0;

   virtual bool add_piece (
       Squares square, Piece::Type piece, Piece::Player player) = 0;

   virtual bool remove_piece (const std::string& location) = 0;
   virtual bool remove_piece (Squares square) = 0;

   virtual Error make_move (Move& move, bool is_computer_move) = 0;
   virtual bool  undo_move () = 0;

   virtual void  label_move (Move& move) const = 0;

   virtual bool is_king_in_check () const = 0;
   virtual bitboard attacks_to (Squares location, bool include_king) const = 0;
   virtual bitboard threats_to (Squares location, Piece::Type type) const = 0;


   virtual bitboard get_moves (Piece::Type piece, Squares square) const = 0;
   virtual bitboard get_all_pieces () const = 0;
   virtual bitboard get_pieces (Piece::Player player) const = 0;
   virtual bitboard get_pieces (Piece::Player player, Piece::Type piece) const = 0;

   virtual bool is_en_passant_on () const = 0;
   virtual bool can_castle (Piece::Player player, CastleSide side) const = 0;
   virtual bool is_castled (Piece::Player player, CastleSide side) const = 0;

   virtual bitboard get_en_passant_square () const = 0;
   virtual Squares get_initial_king_square (Piece::Player player) const = 0;

   virtual Piece::Player get_piece_color (Squares square) const = 0;
   virtual Piece::Player get_turn () const = 0;
   virtual Piece::Type get_piece (Squares square) const = 0;
   virtual ullong get_hash_key () const = 0;
   virtual ullong get_hash_lock () const = 0;
   virtual uint get_move_number () const = 0;
   virtual ushort get_repetition_count () const = 0;

   virtual void set_game_status (GameStatus status) = 0;
   virtual void set_en_passant (Squares en_passant_capture_square) = 0;
   virtual void set_turn (Piece::Player player) = 0;
   virtual void set_castling_privilege (
       Piece::Player player, CastleSide side, bool value) = 0;

   static bool is_inside_board (int row, int col);
   static bool is_inside_board (uint row, uint col);
   static bool is_inside_board (uint square);

   friend std::ostream& operator << (std::ostream& out, const Board& board);
};

inline Board::Squares&
operator ++ (Board::Squares& square)
{
   return (square = Board::Squares (square + 1));
}

} // namespace game_rules

#endif // BOARD_H
