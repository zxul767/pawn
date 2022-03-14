#ifndef BISHOP_H
#define BISHOP_H

/*==============================================================================
  Encodes the move rules for a bishop, both in its general form (i.e. assumming
  an empty board), and in specific situations (i.e. in a board with pieces)
  ==============================================================================*/

#include "BoardTraits.hpp"
#include "GameTraits.hpp"
#include "Piece.hpp"

namespace game_rules
{
class IBoard;

class Bishop : public Piece
{
  public:
    Bishop();
    ~Bishop();

    bitboard get_moves(uint square, Player player, const IBoard *board) const;
    bitboard get_potential_moves(uint square, Player player) const;

  private:
    bitboard get_diagonal_ray_from(BoardSquare square, Diagonal direction) const;
    void compute_moves();

    bitboard moves_from[BOARD_SQUARES_COUNT][Piece::RAY_DIRECTIONS_COUNT];
    bitboard all_moves_from[BOARD_SQUARES_COUNT];
};

} // namespace game_rules

#endif // BISHOP_H
