#ifndef KNIGHT_H
#define KNIGHT_H

/*==============================================================================
  Encodes the move rules for a knight, both in its general form (i.e. assumming
  an empty board), and in specific situations (i.e. in a board with pieces)
  ==============================================================================*/

#include "GameTraits.hpp"
#include "Piece.hpp"

namespace rules
{
class IBoard;

class Knight : public Piece
{
  public:
    Knight();
    ~Knight();

    bitboard get_moves(uint square, Player, const IBoard *) const;
    bitboard get_potential_moves(uint square, Player) const;

  private:
    void compute_moves();

    bitboard moves_from[BOARD_SQUARES_COUNT];
};

} // namespace rules

#endif // KNIGHT_H
