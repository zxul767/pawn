#ifndef QUEEN_H
#define QUEEN_H

/*==============================================================================
  Encodes the move rules for a queen, both in its general form (i.e. assumming
  an empty board), and in specific situations (i.e. in a board with pieces)
 ==============================================================================*/
#include "Piece.hpp"

namespace rules
{
class Rook;
class Bishop;

class Queen : public Piece
{
  public:
    Queen();
    ~Queen();

    bitboard get_moves(uint square, Player player, const IBoard *board) const;
    bitboard get_potential_moves(uint square, Player player) const;

  private:
    // Queen's moves are simply the combination of Rook and Bishop's moves
    Rook *rook;
    Bishop *bishop;
};

} // namespace rules

#endif // QUEEN_H
