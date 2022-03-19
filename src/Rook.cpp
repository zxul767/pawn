#include "Rook.hpp"
#include "IBoard.hpp"

namespace rules
{
Rook::Rook()
{
    compute_moves();
}

Rook::~Rook()
{
}
/*=============================================================================
  Get all moves from SQUARE in the current BOARD assumming it is PLAYER'S turn
  to move (moves that may leave the king in check are also included)

  TODO: write a high-level description of the algorithm used to find rook valid
  moves
  ============================================================================*/
bitboard Rook::get_moves(uint square, Piece::Player player, const IBoard *board) const
{
    bitboard attacks = 0;
    bitboard blocking_pieces;
    BoardSquare first_blocking_piece;

    // Compute attacks to each of the possible directions a rook can point to
    bitboard all_pieces = board->get_all_pieces();
    for (RowColumn ray = NORTH; ray <= WEST; ++ray)
    {
        blocking_pieces = get_ray_from(BoardSquare(square), ray) & all_pieces;

        if (ray == NORTH || ray == WEST)
            first_blocking_piece = BoardSquare(util::msb_position(blocking_pieces));
        else
            first_blocking_piece = BoardSquare(util::lsb_position(blocking_pieces));

        attacks |= get_ray_from(BoardSquare(square), ray) ^
                   (blocking_pieces ? get_ray_from(first_blocking_piece, ray) : 0);
    }
    attacks &= ~board->get_pieces(player);

    return attacks;
}

/*=============================================================================
  Return all possible moves from SQUARE, assuming the board is empty.
  ============================================================================*/
bitboard
/* Only for pawns is the player to move relevant in computing the potential moves */
Rook::get_potential_moves(uint square, Player /* player */) const
{
    if (IBoard::is_inside_board(square))
        return this->all_moves_from[square];

    return 0;
}

/*=============================================================================
  Compute all moves a bishop can make from every square on the board assuming
  the board is empty.
  ============================================================================*/
void Rook::compute_moves()
{
    int dx[Piece::RAY_DIRECTIONS_COUNT] = {0, +1, 0, -1};
    int dy[Piece::RAY_DIRECTIONS_COUNT] = {-1, 0, +1, 0};

    for (BoardSquare square = BoardSquare::a8; square <= BoardSquare::h1; ++square)
    {
        for (uint ray = 0; ray < Piece::RAY_DIRECTIONS_COUNT; ++ray)
            this->moves_from[square][ray] = 0;

        this->all_moves_from[square] = 0;
    }

    for (uint row = 0; row < BOARD_SIZE; ++row)
        for (uint col = 0; col < BOARD_SIZE; ++col)
        {
            auto square = BoardSquare(row * BOARD_SIZE + col);
            RowColumn ray;

            /*--------------------------------------------------------------------
              Traverse all four directions a rook can move to

                    N
                    |           N : North
              W ____|____ E     E : East
                    |           S : South
                    |           W : West
                    S
              -------------------------------------------------------------------*/
            for (ray = Piece::NORTH; ray <= Piece::WEST; ++ray)
            {
                int y = row;
                int x = col;
                while (IBoard::is_inside_board(y + dy[ray], x + dx[ray]))
                {
                    y += dy[ray];
                    x += dx[ray];
                    this->moves_from[square][ray] |=
                        (util::constants::ONE << (y * BOARD_SIZE + x));
                }
                this->all_moves_from[square] |= this->moves_from[square][ray];
            }
        }
}

/*=============================================================================
  Return all possible moves from SQUARE following DIRECTION, assuming the
  board is empty.
  ============================================================================*/
bitboard Rook::get_ray_from(BoardSquare square, RowColumn direction) const
{
    if (IBoard::is_inside_board(square))
        return this->moves_from[square][direction];

    return 0;
}

} // namespace rules
