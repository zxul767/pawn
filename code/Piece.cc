#include "Piece.h"

string
Piece::pieceString (Type piece_type)
{
   const static string piece[] = { "Pawn", "Knight", "Bishop", 
                                   "Rook", "Queen", "King", "NULL"};

   return piece[piece_type];
}
