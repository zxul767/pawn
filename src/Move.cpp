#include "Move.hpp"
#include "BoardTraits.hpp"
#include "GameTraits.hpp"
#include "IBoard.hpp"
#include <cctype>
#include <cstdlib>

namespace rules
{
using std::string;

/*=============================================================================
  Create a move given an algebraic chess notation.

  For instance, the notation e2e4 denotes the movement of the king's pawn from
  its initial square to the center of the board.
  ============================================================================*/
Move::Move(const string &move_notation)
{
    if (!translate_to_square(move_notation.substr(0, 2), start) ||
        !translate_to_square(move_notation.substr(2, 2), end))
    {
        // Create a null move if NOTATION was incorrect
        this->_score = 0;
        this->start = this->end = BoardSquare::a8;
        this->_type = NULL_MOVE;
        this->_moving_piece = Piece::NULL_PIECE;
        this->_captured_piece = Piece::NULL_PIECE;
    }
}

/*=============================================================================
  Create a dummy move.

  This constructor is called when we declare a variable of type Move just to
  assign a value to it from another one.
  =============================================================================*/
Move::Move()
{
    this->_score = 0;
    this->start = this->end = BoardSquare::a1;
    this->_type = NULL_MOVE;
    this->_moving_piece = Piece::NULL_PIECE;
    this->_captured_piece = Piece::NULL_PIECE;
}

/*=============================================================================
  Create a move without specifying its type.

  This is done this way  since the kind of move is usually known only after some
  processing which requires only the START and END squares
  ============================================================================*/
Move::Move(BoardSquare start, BoardSquare end)
{
    this->_score = 0;
    this->start = start;
    this->end = end;
    this->_type = NULL_MOVE;
    this->_moving_piece = Piece::NULL_PIECE;
    this->_captured_piece = Piece::NULL_PIECE;
}

/*=============================================================================
  Return TRUE if THIS is a null move; return FALSE otherwise
  ============================================================================*/
bool Move::is_null() const
{
    return this->_type == NULL_MOVE;
}

/*=============================================================================
  Output information regarding MOVE to the stream OUT
  ============================================================================*/
std::ostream &operator<<(std::ostream &out, const Move &move)
{
    if (move.type() != Move::NULL_MOVE)
    {
        string initial, final;
        Move::translate_to_notation(move.from(), initial);
        Move::translate_to_notation(move.to(), final);

        if (move.moving_piece() != Piece::NULL_PIECE)
        {
            out << Piece::pieceString(move.moving_piece());
        }
        out << "\t" << initial << " - " << final;
    }
    return out;
}

bool operator==(const Move &m1, const Move &m2)
{
    return (m1.start == m2.start && m1.end == m2.end);
}

bool operator<(const Move &m1, const Move &m2)
{
    return m1._score < m2._score;
}

BoardSquare Move::from() const
{
    return this->start;
}

BoardSquare Move::to() const
{
    return this->end;
}

Move::Type Move::type() const
{
    return this->_type;
}

void Move::set_type(Move::Type type)
{
    this->_type = type;
}

Piece::Type Move::moving_piece() const
{
    return this->_moving_piece;
}

Piece::Type Move::captured_piece() const
{
    return this->_captured_piece;
}

void Move::set_moving_piece(Piece::Type piece)
{
    this->_moving_piece = piece;
}

void Move::set_captured_piece(Piece::Type piece)
{
    this->_captured_piece = piece;
}

void Move::set_score(int score)
{
    this->_score = score;
}

int Move::score() const
{
    return this->_score;
}

/*=============================================================================
  Return TRUE if NOTATION was correctly translated into a number in the range
  [0 .. BOARD_SQUARES_COUNT); return FALSE otherwise.

  The function mapping chess notation to squares is as follows:

  f(A8) -> 0, f(H8) -> 7,
  f(A7) -> 8, ...
  ...
  ...
  f(A1) -> 56, f(H1) -> 63
  ============================================================================*/
bool Move::translate_to_square(const string &notation, BoardSquare &square)
{
    if (is_valid_notation(notation))
    {
        ushort column = toupper(notation[0]) - 'A';
        ushort row = (BOARD_SIZE - 1) - (notation[1] - '1');

        square = (BoardSquare)(row * BOARD_SIZE + column);
        return true;
    }
    return false;
}

/*=============================================================================
  Inverse function of translate_to_square
  ============================================================================*/
bool Move::translate_to_notation(BoardSquare square, string &notation)
{
    if (!IBoard::is_inside_board(square))
        return false;

    notation.clear();
    notation += (char)('a' + (square % BOARD_SIZE));
    notation += (char)('8' - (square / BOARD_SIZE));

    return true;
}

/*=============================================================================
  Return TRUE if NOTATION represents a valid algebraic chess notation square;
  return FALSE otherwise.

  For instance, A8 is a valid notation, whereas B10 is not.
  ============================================================================*/
bool Move::is_valid_notation(const string &notation)
{
    if (notation.length() != 2)
        return false;

    if (toupper(notation[0]) >= 'A' && toupper(notation[0]) <= 'H')
        if (notation[1] >= '1' && notation[1] <= '8')
            return true;

    return false;
}

} // namespace rules
