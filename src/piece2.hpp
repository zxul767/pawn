#ifndef PIECE2_H_
#define PIECE2_H_

namespace rules
{
struct BoardPiece
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

    const static BoardPiece WHITE_ROOK;
    const static BoardPiece WHITE_KNIGHT;
    const static BoardPiece WHITE_BISHOP;
    const static BoardPiece WHITE_KING;
    const static BoardPiece WHITE_QUEEN;
    const static BoardPiece WHITE_PAWN;
    const static BoardPiece BLACK_ROOK;
    const static BoardPiece BLACK_KNIGHT;
    const static BoardPiece BLACK_BISHOP;
    const static BoardPiece BLACK_KING;
    const static BoardPiece BLACK_QUEEN;
    const static BoardPiece BLACK_PAWN;
    const static BoardPiece NULL_PIECE;

    bool operator==(const BoardPiece &other) const;
    bool is_null() const;

    Kind kind;
    Color color;
};

BoardPiece::Kind operator++(BoardPiece::Kind &kind);
BoardPiece::Color operator++(BoardPiece::Color &color);

} // namespace rules

#endif // PIECE2_H_
