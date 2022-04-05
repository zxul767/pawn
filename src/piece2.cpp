#include "piece2.hpp"

namespace rules
{
const Piece Piece::WHITE_PAWN = {.kind = PAWN, .color = WHITE};
const Piece Piece::WHITE_ROOK = {.kind = ROOK, .color = WHITE};
const Piece Piece::WHITE_KNIGHT = {.kind = KNIGHT, .color = WHITE};
const Piece Piece::WHITE_BISHOP = {.kind = BISHOP, .color = WHITE};
const Piece Piece::WHITE_KING = {.kind = KING, .color = WHITE};
const Piece Piece::WHITE_QUEEN = {.kind = QUEEN, .color = WHITE};

const Piece Piece::BLACK_PAWN = {.kind = PAWN, .color = BLACK};
const Piece Piece::BLACK_ROOK = {.kind = ROOK, .color = BLACK};
const Piece Piece::BLACK_KNIGHT = {.kind = KNIGHT, .color = BLACK};
const Piece Piece::BLACK_BISHOP = {.kind = BISHOP, .color = BLACK};
const Piece Piece::BLACK_KING = {.kind = KING, .color = BLACK};
const Piece Piece::BLACK_QUEEN = {.kind = QUEEN, .color = BLACK};

const Piece Piece::NULL_PIECE = {.kind = NULL_KIND, .color = NULL_COLOR};

Piece::Color operator++(Piece::Color &color)
{
    if (color == Piece::NULL_COLOR)
        return color;
    return (color = static_cast<Piece::Color>(static_cast<int>(color) + 1));
}

Piece::Kind operator++(Piece::Kind &kind)
{
    if (kind == Piece::NULL_KIND)
        return kind;
    return (kind = static_cast<Piece::Kind>(static_cast<int>(kind) + 1));
}

bool Piece::operator==(const Piece &other) const
{
    return (other.kind == this->kind && other.color == this->color);
}

bool Piece::is_null() const
{
    return this->color == NULL_COLOR && this->kind == NULL_KIND;
};

} // namespace rules
