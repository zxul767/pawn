/*=============================================================================
  Class name: Pawn

  Responsabilities: 
  -Provide the set of moves a pawn can make from any square on the board.

  Collaborations:

  Version: 0.1

  Date: September 01, 2007
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

   bitboard get_moves (u_int square, Player player, const Board* board) const;

   bitboard get_side_moves   (u_int square, Player player) const;
   bitboard get_double_move  (u_int square, Player player) const;
   bitboard get_capture_move (u_int square, Player player, 
                              RowColumn direction) const;

   bitboard get_potential_moves (u_int square, Player player) const;

 private:

   bitboard get_simple_moves (u_int square, Player player) const;

   void     compute_moves ();
   bitboard compute_simple_moves (u_int square, Player player) const;
   bitboard compute_side_moves   (u_int square, Player player) const;
   bitboard compute_capture_move (u_int square, Player player, 
                                  RowColumn direction) const;

   bool  is_second_row (u_int row, Player player) const;
   bool  is_valid_row  (u_int row, Player player) const;
   u_int get_row       (u_int square) const;
   u_int get_column    (u_int square) const;
   
   bitboard moves_from[Board::SQUARES][Board::PLAYERS];
   bitboard simple_moves_from[Board::SQUARES][Board::PLAYERS];
   bitboard capture_moves_from[Board::SQUARES][Board::PLAYERS][PAWN_MOVES-1];

   // These are not real moves, but they are useful for en-passant handling
   bitboard side_moves_from[Board::SQUARES][Board::PLAYERS];
};

#endif
