#include "Board.h"
#include "SimpleEvaluator.h"

SimpleEvaluator::SimpleEvaluator () 
{
   int KING_VALUE = Util::INFINITUM;

   piece_value.push_back (100); // PAWN
   piece_value.push_back (300); // KNIGHT
   piece_value.push_back (325); // BISHOP
   piece_value.push_back (500); // ROOK
   piece_value.push_back (900); // QUEEN
   piece_value.push_back (KING_VALUE); // KING

   factor_weight.push_back (99); // MATERIAL
   factor_weight.push_back (12); // MOBILITY
   factor_weight.push_back (60); // CENTER_CONTROL
   factor_weight.push_back (10); // KING_SAFETY
}

int 
SimpleEvaluator::static_evaluation (const Board* board) const
{
   bitboard  player_piece;
   bitboard  opponent_piece;
   int       material = 0;
   int       mobility = 0;
   int       king_security; 
   int       center_control = 0;
   //   int       development = 0;

   int      sign = (board->get_turn () == Piece::WHITE ? 1 : -1);

   // MATERIAL AND MOBILITY VALUES
   for (Piece::Type piece = Piece::PAWN; piece <= Piece::QUEEN; ++piece)
   {
      player_piece   = board->get_pieces (Piece::WHITE, piece);
      opponent_piece = board->get_pieces (Piece::BLACK, piece);
      
      material += (material_value (player_piece, piece) -
                   material_value (opponent_piece, piece));

      mobility += (mobility_value (board, player_piece, piece) -
                   mobility_value (board, opponent_piece, piece));

      center_control += (center_control_value (board, player_piece, piece) -
                         center_control_value (board, opponent_piece, piece));
   }
   
   king_security = (king_safety (board, Piece::WHITE) - 
                    king_safety (board, Piece::BLACK));

   return sign * (factor_weight[MATERIAL] * material + 
                  factor_weight[MOBILITY] * mobility + 
                  factor_weight[CENTER_CONTROL] * center_control +
                  factor_weight[KING_SAFETY] * king_security);
}

int 
SimpleEvaluator::material_value (bitboard piece, Piece::Type piece_type) const
{
   u_int counter = Util::count_set_bits (piece);
   return (counter * piece_value[piece_type]);
}

int 
SimpleEvaluator::mobility_value (const Board* board, 
                                 bitboard     piece, 
                                 Piece::Type  piece_type) const
{   
   bitboard  moves = 0;
   uint      n_moves = 0;

   int position = Util::MSB_position (piece);
   while (piece && position != -1)
   {
      moves    = board->get_moves (piece_type, Board::Squares (position));
      n_moves += Util::count_set_bits (moves);
      piece   ^= Util::one << position;
      position = Util::MSB_position (piece);
   }

   if (board->get_move_number () <= 10 && piece_type == Piece::QUEEN)
      return 0;

   return n_moves;
}

int
SimpleEvaluator::center_control_value (const Board* board, 
                                       bitboard     piece,
                                       Piece::Type  piece_type) const
{
   bitboard center = 
      (Util::one << Board::e4) | 
      (Util::one << Board::d4) | 
      (Util::one << Board::e5) | 
      (Util::one << Board::d5);

   bitboard attacks;
   int      squares_controled = 0;

   squares_controled += (Util::count_set_bits (piece & center));

   int position = Util::MSB_position (piece);
   while (piece && position != -1)
   {
      attacks = board->get_moves (piece_type, Board::Squares (position));
      squares_controled += Util::count_set_bits (attacks & center);
      piece ^= Util::one << position;
      position = Util::MSB_position (piece);
   }

   return (squares_controled);
}

int
SimpleEvaluator::king_safety (const Board*  board, 
                              Piece::Player player) const
{
   static bitboard pawns[Board::PLAYERS][Board::PLAYERS] = 
      {{Util::to_bitboard[Board::f2] |
        Util::to_bitboard[Board::g2] |
        Util::to_bitboard[Board::h2],
        Util::to_bitboard[Board::a2] |
        Util::to_bitboard[Board::b2] |
        Util::to_bitboard[Board::c2]}, 
       
       {Util::to_bitboard[Board::f7] |
        Util::to_bitboard[Board::g7] |
        Util::to_bitboard[Board::h7],
        Util::to_bitboard[Board::a7] |
        Util::to_bitboard[Board::b7] |
        Util::to_bitboard[Board::c7]}};

   int  safety_value = 0;
   bool has_moved; // king has moved from its original square

   has_moved = false;
   if (!((Util::one << board->get_initial_king_square (player)) &
         board->get_pieces (player, Piece::KING)))
      has_moved = true;


   if (board->is_castled (player, Board::KING_SIDE))
   {
      if ((pawns[player][Board::KING_SIDE] & 
           board->get_pieces (player, Piece::PAWN) == pawns[player][Board::KING_SIDE]))
         safety_value = 20;
      else
         safety_value = -40;
   }
   else if (board->is_castled (player, Board::QUEEN_SIDE))
   {
      if ((pawns[player][Board::QUEEN_SIDE] & 
           board->get_pieces (player, Piece::PAWN) == pawns[player][Board::QUEEN_SIDE]))
         safety_value = 20;
      else
         safety_value = -40;
   }
   else if (has_moved) safety_value = -50;
   else                safety_value = -20;

   return safety_value;
}

int
SimpleEvaluator::get_piece_value (Piece::Type piece_type) const
{
   return piece_value[piece_type];
}

void
SimpleEvaluator::load_factor_weights (vector<int>& weights)
{
   for (u_int i = 0; i < weights.size (); ++i)
   {
      if (i >= factor_weight.size ())
         break;
      factor_weight[i] = weights[i];
   }
}

