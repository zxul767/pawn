#include "Piece.hpp"

namespace game_rules
{
std::string Piece::pieceString(Type piece_type)
{
    const static std::string piece[] = {"Pawn",  "Knight", "Bishop", "Rook",
                                        "Queen", "King",   "NULL"};

    return piece[piece_type];
}

} // namespace game_rules
