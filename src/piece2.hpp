#ifndef PIECE2_H_
#define PIECE2_H_

namespace rules
{
struct Piece
{
  public:
    static const int PIECES_COUNT = 6;
    static const int COLORS_COUNT = 2;

    enum Kind
    {
        PAWN = 0,
        KNIGHT = 1,
        BISHOP = 2,
        ROOK = 3,
        QUEEN = 4,
        KING = 5,
        NULL_KIND = 6
    };

    enum Color
    {
        WHITE = 0,
        BLACK = 1,
        NULL_COLOR = 2
    };

    const static Piece WHITE_ROOK;
    const static Piece WHITE_KNIGHT;
    const static Piece WHITE_BISHOP;
    const static Piece WHITE_KING;
    const static Piece WHITE_QUEEN;
    const static Piece WHITE_PAWN;
    const static Piece BLACK_ROOK;
    const static Piece BLACK_KNIGHT;
    const static Piece BLACK_BISHOP;
    const static Piece BLACK_KING;
    const static Piece BLACK_QUEEN;
    const static Piece BLACK_PAWN;
    const static Piece NULL_PIECE;

    bool operator==(const Piece &other) const;
    bool is_null() const;

    Kind kind;
    Color color;
};

Piece::Kind operator++(Piece::Kind &kind);
Piece::Color operator++(Piece::Color &color);

} // namespace rules

#endif // PIECE2_H_
