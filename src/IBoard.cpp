/*==============================================================================
  Defines a few common operations for the base class. See IBoard.h to see the
  interface specification.
  ==============================================================================*/

#include "IBoard.hpp"
#include "BoardTraits.hpp"
#include "GameTraits.hpp"

namespace game_rules
{
IBoard::IBoard()
{
}

IBoard::~IBoard()
{
}

bool IBoard::is_inside_board(int row, int col)
{
    return (row >= 0 && row < (int)BOARD_SIZE) && (col >= 0 && col < (int)BOARD_SIZE);
}

bool IBoard::is_inside_board(uint row, uint col)
{
    return (row < BOARD_SIZE) && (col < BOARD_SIZE);
}

bool IBoard::is_inside_board(uint square)
{
    return (square < BOARD_SQUARES_COUNT);
}

std::ostream &operator<<(std::ostream &out, const IBoard &board)
{
    static const std::string piece_to_string[PIECE_KINDS_COUNT][PLAYERS_COUNT] = {
        {"  P  ", " *P* "}, {"  N  ", " *N* "}, {"  B  ", " *B* "},
        {"  R  ", " *R* "}, {"  Q  ", " *Q* "}, {"  K  ", " *K* "},
    };

    out << "  -------------------------------------------------" << std::endl;
    for (uint row = 0; row < BOARD_SIZE; ++row)
    {
        out << "  |     |     |     |     |     |     |     |     |" << std::endl;
        out << (BOARD_SIZE - row) << ' ';

        for (uint col = 0; col < BOARD_SIZE; ++col)
        {
            out << '|';

            auto square = (BoardSquare)(row * BOARD_SIZE + col);
            Piece::Player player = board.get_piece_color(square);
            Piece::Type piece = board.get_piece(square);

            if ((player != Piece::NULL_PLAYER) && (piece != Piece::NULL_PIECE))
            {
                out << piece_to_string[piece][player];
            }
            else
                // If there is nothing in (row, col) just output an empty square
                out << "     ";
        }
        out << '|' << std::endl;
        out << "  -------------------------------------------------" << std::endl;
    }

    // Display algebraic coordinates
    out << "     a     b     c     d     e     f     g     h   " << std::endl;

    return out;
}

} // namespace game_rules
