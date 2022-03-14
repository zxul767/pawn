#ifndef MOVE_H
#define MOVE_H

#include "BoardTraits.hpp"
#include "Piece.hpp"
#include <string>

namespace game_rules
{
class Move
{
  public:
    Move();
    Move(const std::string &notation);
    Move(BoardSquare start, BoardSquare end);
    Move(const Move &) = default;

    enum Type
    {
        SIMPLE_MOVE,
        NORMAL_CAPTURE,
        CHECK,
        CASTLE_KING_SIDE,
        CASTLE_QUEEN_SIDE,
        PROMOTION_MOVE,
        EN_PASSANT_CAPTURE,
        NULL_MOVE
    };

    BoardSquare from() const;
    BoardSquare to() const;

    Type get_type() const;
    void set_type(Type type);

    Piece::Type get_moving_piece() const;
    Piece::Type get_captured_piece() const;

    void set_moving_piece(Piece::Type piece);
    void set_captured_piece(Piece::Type piece);

    void set_score(int score);
    int get_score() const;

    bool is_null() const;

    friend std::ostream &operator<<(std::ostream &out, const Move &move);

    friend bool operator==(const Move &m1, const Move &m2);
    friend bool operator<(const Move &m1, const Move &m2);

    static bool translate_to_square(const std::string &notation, BoardSquare &square);
    static bool translate_to_notation(BoardSquare square, std::string &notation);
    static bool is_valid_notation(const std::string &notation);

  private:
    BoardSquare start;
    BoardSquare end;
    Type type;
    Piece::Type moving_piece;
    Piece::Type captured_piece;

    // The score assigned to this move is relative to the position of the board
    int score;
};

} // namespace game_rules

#endif // MOVE_H
