#ifndef PAWN_H
#define PAWN_H

/*==============================================================================
  Encodes the move rules for a pawn, both in its general form (i.e. assumming
  an empty board), and in specific situations (i.e. in a board with pieces)
  ==============================================================================*/

#include "GameTraits.hpp"
#include "Piece.hpp"

namespace rules
{
class IBoard;

class Pawn : public Piece
{
  public:
    Pawn();
    ~Pawn();

    bitboard get_moves(uint square, Player player, const IBoard *board) const;

    bitboard get_side_moves(uint square, Player player) const;
    bitboard get_double_move(uint square, Player player) const;
    bitboard get_capture_move(uint square, Player player, RowColumn direction) const;

    bitboard get_potential_moves(uint square, Player player) const;

  private:
    bitboard get_simple_moves(uint square, Player player) const;

    void compute_moves();
    bitboard compute_simple_moves(uint square, Player player) const;
    bitboard compute_side_moves(uint square, Player player) const;
    bitboard compute_capture_move(uint square, Player player, RowColumn direction) const;

    bool is_second_row(uint row, Player player) const;
    bool is_valid_row(uint row, Player player) const;
    uint get_row(uint square) const;
    uint get_column(uint square) const;

    bitboard moves_from[BOARD_SQUARES_COUNT][PLAYERS_COUNT];
    bitboard simple_moves_from[BOARD_SQUARES_COUNT][PLAYERS_COUNT];
    bitboard capture_moves_from[BOARD_SQUARES_COUNT][PLAYERS_COUNT]
                               [PAWN_CAPTURE_MOVES_COUNT];

    // These are not real moves, but they are useful for en-passant handling
    bitboard side_moves_from[BOARD_SQUARES_COUNT][PLAYERS_COUNT];
};

} // namespace rules

#endif // PAWN_H
