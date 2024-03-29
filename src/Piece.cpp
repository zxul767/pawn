#include "Piece.hpp"

namespace rules
{
std::string Piece::pieceString(Type piece_type)
{
    const static std::string piece[] = {"Pawn",  "Knight", "Bishop", "Rook",
                                        "Queen", "King",   "NULL"};

    return piece[piece_type];
}

} // namespace rules
