#ifndef SQUARE_H
#define SQUARE_H

#include "Piece.hpp"

namespace rules
{
class Square
{
  public:
    Piece::Player player;
    Piece::Type piece;

    bool operator==(const Square &other) const
    {
        return (other.player == this->player && other.piece == this->piece);
    }

    bool operator!=(const Square &other) const
    {
        return (other.player != this->player || other.piece != this->piece);
    }
};

} // namespace rules

#endif // SQUARE_H
