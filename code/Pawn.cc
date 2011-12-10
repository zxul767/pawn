/*=============================================================================
  Class name: Pawn

  Responsabilities: 
  -Provide the set of moves a pawn can make from any square on the board.

  Collaborations:

  Version: 0.1

  Date: September 01, 2007
  =============================================================================*/

#include "Pawn.h"

Pawn:: Pawn ()
{       
   compute_moves ();
}

Pawn::~Pawn ()
{
}

/*============================================================================
  Return all valid moves from SQUARE in the current BOARD, assuming it is 
  PLAYER's turn to move (moves that leave the king in check are also included)
  ============================================================================*/
bitboard
Pawn::get_moves (uint square, Player player, const Board* board) const
{
   Piece::Player opponent = WHITE;

   if (!Board::is_inside_board (square))
      return 0;

   if (player == WHITE)
      opponent = BLACK;

   bitboard all_pieces = board->get_all_pieces ();
   bitboard opponent_pieces = board->get_pieces (opponent);
   bitboard opponent_pawns = board->get_pieces (opponent, PAWN);

   bitboard east_capture = get_capture_move (square, player, Piece::EAST);
   bitboard west_capture = get_capture_move (square, player, Piece::WEST);
   bitboard captures = east_capture | west_capture;

   bitboard simple_moves = simple_moves_from [square][player];
   bitboard moves = 
      (captures & opponent_pieces) | 
      (simple_moves & ~all_pieces);

   bitboard double_move = get_double_move (square, player);

   // Is there a two-square move included already ? [*]
   if ((moves & double_move) &&
       // If there is a piece in the way to making a two-square move, you
       // must discard that move
       (simple_moves ^ double_move) & all_pieces)
   {
      // [*] Failing to check this will only add an invalid move to the set
      moves ^= double_move;
   }

   // Add possible en-passant captures
   if (board->is_en_passant_on () &&
       (get_side_moves (square, player) & opponent_pawns))
   {
      bitboard
      en_passant_capture_square = board->get_en_passant_square ();

      if (west_capture & en_passant_capture_square)
         moves |= en_passant_capture_square;

      else if (east_capture & en_passant_capture_square)
         moves |= en_passant_capture_square;
   }

   return moves;
}

/*=============================================================================
  Return all possible moves from SQUARE, assuming the board is empty.
  ============================================================================*/
bitboard
Pawn::get_potential_moves (uint square, Player player) const
{
   if (Board::is_inside_board (square))
      return moves_from[square][player];   
   return 0;
}


/*=============================================================================
  Return a potential square a pawn on SQUARE can capture following DIRECTION,
  and assuming it is PLAYER's turn to play.
  =============================================================================*/
bitboard 
Pawn::get_capture_move (uint     square, 
                        Player    player, 
                        RowColumn direction) const
{
   if (Board::is_inside_board (square))
      if (direction == EAST || direction == WEST)
      {
         uint side = (direction == EAST ? 0 : 1);
         return capture_moves_from[square][player][side];
      }
   return 0;
}

/*=============================================================================
  Return all non-capture moves that a pawn on SQUARE can make, assuming it is
  PLAYER's turn to play.
  =============================================================================*/
bitboard 
Pawn::get_simple_moves (uint square, Player player) const
{
   if (Board::is_inside_board (square))
      return simple_moves_from[square][player];
   return 0;
}

/*=============================================================================
  Return the end square of a two-squares move for a pawn, if SQUARE is on the
  second row for PLAYER
  =============================================================================*/
bitboard
Pawn::get_double_move (uint square, Player player) const
{  
   if (!Board::is_inside_board (square))
      return 0;

   uint offset = Board::SIZE + Board::SIZE;
   uint end_square;

   if (is_second_row (get_row (square), player)) 
   {
      if (player == Piece::WHITE)
         end_square = square - offset;
      else
         end_square = square + offset;

      return (Util::one << (end_square));
   }
   return 0;
}

/*=============================================================================
  Return the horizontal adjacent squares to SQUARE. This is useful in checking
  legality of en-passant captures, as well as on setting the en-passant flag.
  =============================================================================*/
bitboard 
Pawn::get_side_moves (uint square, Player player) const
{
   if (Board:: is_inside_board (square))
      return side_moves_from[square][player];
   return 0;
}

/*=============================================================================
  Compute all moves a pawn can make from every square on the board assuming
  the board is empty. Moves are computed for both WHITE and BLACK pawns.
  =============================================================================*/       
void
Pawn:: compute_moves ()
{   
   for (uint row = 0; row < Board::SIZE; ++row)
      for (uint col = 0; col < Board::SIZE; ++col)
      {
         uint square = row * Board::SIZE + col;
         
         for (Player player = WHITE; player <= BLACK; ++player)
         {
            side_moves_from[square][player] = compute_side_moves (square, player);
            moves_from[square][player] = 0;

            simple_moves_from[square][player] = 
               compute_simple_moves (square, player);

            capture_moves_from[square][player][0] = 
               compute_capture_move (square, player, EAST);

            capture_moves_from[square][player][1] = 
               compute_capture_move (square, player, WEST);

            // OR simple and capture moves into general moves
            moves_from[square][player] |= simple_moves_from[square][player];

            moves_from[square][player] |= 
               capture_moves_from[square][player][0];

            moves_from[square][player] |= 
               capture_moves_from[square][player][1];
            
            // If potential moves include side moves then:
            // moves_from[square][player] |= side_moves_from[square];
         }
      }
}

/*=============================================================================
  Compute a single bitboard containing the horizontal adjacent squares to
  SQUARE
  =============================================================================*/
bitboard
Pawn::compute_side_moves (uint square, Player player) const
{
   bitboard side_moves = 0;
   int      dx[PAWN_MOVES-1] = { -1, +1 };
   
   int column = (int) get_column (square);
   int row = (int) get_row (square);
   
   for (uint i = 0; i < PAWN_MOVES-1; i++)
   {
      int x = column + dx[i];
      if (Board::is_inside_board (row, x) && is_valid_row (row, player))
         side_moves |= (Util::one << (row * Board::SIZE + x));
   }
   return side_moves;
}

/*=============================================================================
  Compute a single bitboard containing the capture square in DIRECTION, of a 
  pawn on SQUARE, assuming it is PLAYER'S turn to move.
  =============================================================================*/
bitboard
Pawn::compute_capture_move (uint square,
                            Player player,
                            RowColumn direction) const
{
   if (direction != EAST && direction != WEST)
      return 0;

   bitboard capture_move = 0;
   int      dx[PAWN_MOVES-1] = { -1, +1 };
   int      dy[PAWN_MOVES-1] = { +1, +1 };
   
   int column = (int) get_column (square);
   int row = (int) get_row (square);        
   uint position = (direction == EAST? 1: 0);
   
   int x = column + dx[position];
   int y = row + (player == WHITE ? -dy[position]: dy[position]);
   
   if (Board::is_inside_board (y, x))
      capture_move |= (Util::one << (y * Board::SIZE + x));
   
   return capture_move;
}

/*=============================================================================
  Compute a single bitboard containing non-capture moves for a pawn on SQUARE,
  assumming it is PLAYER's turn to move.
  =============================================================================*/
bitboard
Pawn::compute_simple_moves (uint square,
                            Player player) const
{
   bitboard simple_moves = 0;
   int      dy[PAWN_MOVES-1] = { +1, +2 };
   
   int column = (int) get_column (square);
   int row = (int) get_row (square);
   
   // Pawns on their first move can advance two squares
   uint MOVES = is_second_row (row, player) ? 2: 1;
   
   for (uint i = 0; i < MOVES; i++)
   {
      int y = row + (player == WHITE? -dy[i]: dy[i]);
      if (Board::is_inside_board (y, column) && is_valid_row (row, player))
         simple_moves |= (Util::one << (y * Board::SIZE + column));
   }
   
   return simple_moves;
}

/*=============================================================================
  Return TRUE if ROW is the second row from PLAYER's perpective; return FALSE
  otherwise.
  =============================================================================*/
bool
Pawn:: is_second_row (uint row, Player player) const
{
   if (player == WHITE)
      return row == Board::SIZE-2;
                
   return row == 1;
}

/*=============================================================================
  Return the row corresponding to SQUARE on the chess board.
  Precondition: SQUARE is in [0, Board::SQUARES)
  =============================================================================*/
uint
Pawn:: get_row (uint square) const
{  
   return (square / Board::SIZE);
}

/*=============================================================================
  Return the column corresponding to SQUARE on the chess board.
  Precondition: SQUARE is in [0, Board::SQUARES)
  =============================================================================*/
uint
Pawn:: get_column (uint square) const
{   
   return square % Board::SIZE;
}

/*=============================================================================
  Return TRUE if a pawn of side COLOR can be on ROW; return FALSE otherwise.
  =============================================================================*/
bool
Pawn:: is_valid_row (uint row, Player color) const
{
   if (color == WHITE)
      return row != Board::SIZE-1;
   
   return row != 0;
}
