/*=============================================================================
  Class name: Pawn

  Responsabilities:
  -Provide the set of moves a pawn can make from any square on the board.
  =============================================================================*/

#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"
#include "Board.h"

class Pawn : public Piece
{
 public:
   Pawn ();
   ~Pawn ();

   bitboard get_moves (uint square, Player player, const Board* board) const;

   bitboard get_side_moves (uint square, Player player) const;
   bitboard get_double_move (uint square, Player player) const;
   bitboard get_capture_move (uint square, Player player, RowColumn direction) const;

   bitboard get_potential_moves (uint square, Player player) const;

 private:

   bitboard get_simple_moves (uint square, Player player) const;

   void compute_moves ();
   bitboard compute_simple_moves (uint square, Player player) const;
   bitboard compute_side_moves (uint square, Player player) const;
   bitboard compute_capture_move (
       uint square, Player player, RowColumn direction) const;

   bool is_second_row (uint row, Player player) const;
   bool is_valid_row (uint row, Player player) const;
   uint get_row (uint square) const;
   uint get_column (uint square) const;

   bitboard moves_from[Board::SQUARES][Board::PLAYERS];
   bitboard simple_moves_from[Board::SQUARES][Board::PLAYERS];
   bitboard capture_moves_from[Board::SQUARES][Board::PLAYERS][PAWN_MOVES-1];

   // These are not real moves, but they are useful for en-passant handling
   bitboard side_moves_from[Board::SQUARES][Board::PLAYERS];
};

#endif
