#include "Queen.hpp"

namespace rules
{
Queen::Queen()
{
    this->rook = new Rook();
    this->bishop = new Bishop();
}

Queen::~Queen()
{
    delete this->rook;
    delete this->bishop;
}

/*=============================================================================
  Get a bitboard containing all valid moves for a queen in LOCATION, assuming
  it is PLAYER's turn (moves that leave the king in check are also included)
  ===========================================================================*/
bitboard Queen::get_moves(uint square, Player player, const IBoard *board) const
{
    return this->bishop->get_moves(square, player, board) |
           this->rook->get_moves(square, player, board);
}

bitboard Queen::get_potential_moves(uint square, Player player) const
{
    return this->bishop->get_potential_moves(square, player) |
           this->rook->get_potential_moves(square, player);
}

} // namespace rules
