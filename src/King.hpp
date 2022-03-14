#ifndef KING_H
#define KING_H

/*==============================================================================
  Encodes the move rules for a king, both in its general form (i.e. assumming
  an empty board), and in specific situations (i.e. in a board with pieces)
  ==============================================================================*/

#include "BoardTraits.hpp"
#include "GameTraits.hpp"
#include "Piece.hpp"

namespace game_rules
{
class IBoard;

class King : public Piece
{
  public:
    King();
    ~King();

    bitboard get_moves(uint square, Player, const IBoard *) const;
    bitboard get_potential_moves(uint square, Player) const;

    static bitboard get_neighbors(uint position);

  private:
    void compute_moves();
    bitboard moves_from[BOARD_SQUARES_COUNT];

    static bitboard neighbors[BOARD_SQUARES_COUNT];
    static bool compute_neighbors();
    const static bool neighbors_computed;
};

} // namespace game_rules

#endif // KING_H
