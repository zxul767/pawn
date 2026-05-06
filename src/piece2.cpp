#include "piece2.hpp"

namespace rules
{
const BoardPiece BoardPiece::WHITE_PAWN = {.kind = PAWN, .color = WHITE};
const BoardPiece BoardPiece::WHITE_ROOK = {.kind = ROOK, .color = WHITE};
const BoardPiece BoardPiece::WHITE_KNIGHT = {.kind = KNIGHT, .color = WHITE};
const BoardPiece BoardPiece::WHITE_BISHOP = {.kind = BISHOP, .color = WHITE};
const BoardPiece BoardPiece::WHITE_KING = {.kind = KING, .color = WHITE};
const BoardPiece BoardPiece::WHITE_QUEEN = {.kind = QUEEN, .color = WHITE};

const BoardPiece BoardPiece::BLACK_PAWN = {.kind = PAWN, .color = BLACK};
const BoardPiece BoardPiece::BLACK_ROOK = {.kind = ROOK, .color = BLACK};
const BoardPiece BoardPiece::BLACK_KNIGHT = {.kind = KNIGHT, .color = BLACK};
const BoardPiece BoardPiece::BLACK_BISHOP = {.kind = BISHOP, .color = BLACK};
const BoardPiece BoardPiece::BLACK_KING = {.kind = KING, .color = BLACK};
const BoardPiece BoardPiece::BLACK_QUEEN = {.kind = QUEEN, .color = BLACK};

const BoardPiece BoardPiece::NULL_PIECE = {.kind = NULL_KIND, .color = NULL_COLOR};

BoardPiece::Color operator++(BoardPiece::Color &color)
{
    if (color == BoardPiece::NULL_COLOR)
        return color;
    return (color = static_cast<BoardPiece::Color>(static_cast<int>(color) + 1));
}

BoardPiece::Kind operator++(BoardPiece::Kind &kind)
{
    if (kind == BoardPiece::NULL_KIND)
        return kind;
    return (kind = static_cast<BoardPiece::Kind>(static_cast<int>(kind) + 1));
}

bool BoardPiece::operator==(const BoardPiece &other) const
{
    return (other.kind == this->kind && other.color == this->color);
}

bool BoardPiece::is_null() const
{
    return this->color == NULL_COLOR && this->kind == NULL_KIND;
};

} // namespace rules
