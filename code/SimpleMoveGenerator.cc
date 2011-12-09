#include "SimpleMoveGenerator.h"
#include "SimpleEvaluator.h"
#include "Board.h"
#include "common.h"

#include <algorithm>
using std::cerr;

SimpleMoveGenerator::SimpleMoveGenerator () {}

/*==========================================================================
 | Generate all pseudo-legal moves and place captures at the beginning of  |
 | the list, sorted by the Most-Valuable-Victim Least-Valuable-Attacker    |
 | ratio.                                                                  |
 ==========================================================================*/
bool
SimpleMoveGenerator::generate_moves (Board*         board,
                                     vector<Move>&  moves)
{
   SimpleEvaluator  evaluator;
   vector<Move>     captures;
   bitboard         pieces;
   bitboard         valid_moves;
   Piece::Player    player = board->get_turn ();

   // for each piece, add its pseudo-legal moves to the list
   for (Piece::Type piece = Piece::PAWN; piece <= Piece::KING; ++piece)
   {
      // get pieces of a certain type on the board
      pieces = board->get_pieces (player, piece);
      while (pieces)
      {
         // extract pseudo-legal moves for the current piece
         Board::Squares square = Board::Squares (Util::MSB_position (pieces));
         valid_moves = board->get_moves (piece, square);
         pieces ^= (Util::one << square);

         while (valid_moves)
         {
            Board::Squares current_move = 
               Board::Squares (Util::MSB_position (valid_moves));
            valid_moves ^= (Util::one << current_move);

            Move move (square, current_move);
            move.set_moving_piece (piece);
            board->label_move (move);
            Move::Type move_type = move.get_type ();
            
            if (move_type == Move::NORMAL_CAPTURE ||
                move_type == Move::EN_PASSANT_CAPTURE)
            {
               move.set_captured_piece (board->get_piece (current_move));
               double score = evaluator.get_piece_value (move.get_moving_piece ());
               score /= evaluator.get_piece_value (move.get_captured_piece ());         
               move.set_score ((int)(10 * score));
               captures.push_back (move);               
            }
            else
               moves.push_back (move);
         }
      }
   }
   // Sort captures by Most-Valuable-Victim / Least-Valuable-Attacker ratio
   sort (captures.begin (), captures.end ());

   for (u_int i = 0, n = captures.size (); i < n; ++i)
      moves.insert (moves.begin (), captures[i]);

   return moves.size () != 0;
}

/*==========================================================================
 | Generate pseudo legal moves of the kinds contained in FLAGS, as opposed |
 | to simply generating all moves.                                         |
 ==========================================================================*/
bool
SimpleMoveGenerator::generate_moves (Board*         board, 
                                     vector<Move>&  moves,
                                     u_short        flags)
{
   SimpleEvaluator evaluator;

   vector<Move>    promotions;
   vector<Move>    captures;
   vector<Move>    checks, check_evasions;
   vector<Move>    other_moves;

   bitboard        pieces;
   bitboard        valid_moves;
   Piece::Player   player = board->get_turn ();


   // for each piece, add its pseudo-legal moves to the list
   for (Piece::Type piece = Piece::PAWN; piece <= Piece::KING; ++piece)
   {
      // get pieces of a certain type on the board
      pieces = board->get_pieces (player, piece);
      while (pieces)
      {
         // extract pseudo-legal moves for the current piece
         Board::Squares square = Board::Squares (Util::MSB_position (pieces));
         valid_moves = board->get_moves (piece, square);
         pieces ^= (Util::one << square);

         while (valid_moves)
         {
            Board::Squares current_move = 
               Board::Squares (Util::MSB_position (valid_moves));
            valid_moves ^= (Util::one << current_move);

            Move move (square, current_move);
            move.set_moving_piece (piece);
            board->label_move (move);
            Move::Type move_type = move.get_type ();
            
            if (move_type == Move::NORMAL_CAPTURE ||
                move_type == Move::EN_PASSANT_CAPTURE)
            {
               move.set_captured_piece (board->get_piece (current_move));

               if (flags & MoveGenerator::CAPTURES)
               {
                  double score = evaluator.get_piece_value (move.get_moving_piece ());
                  score /= evaluator.get_piece_value (move.get_captured_piece ());         
                  move.set_score ((int)(10 * score));
                  captures.push_back (move);
               }
            }
            
            if ((flags & MoveGenerator::CHECKS) && move_type == Move::CHECK)
               checks.push_back (move);

            if ((flags & MoveGenerator::PAWN_PROMOTIONS) && 
                move_type == Move::PROMOTION_MOVE)
               promotions.push_back (move);

            if ((flags & MoveGenerator::SIMPLE) && 
                (move_type == Move::SIMPLE_MOVE ||
                 move_type == Move::CASTLE_KING_SIDE ||
                 move_type == Move::CASTLE_QUEEN_SIDE))
               other_moves.push_back (move);

            if ((flags & MoveGenerator::CHECK_EVASIONS) &&
                board->is_king_in_check ())
            {
               Board::Error error = board->make_move (move, true);
               if (error == Board::NO_ERROR)
               {
                  check_evasions.push_back (move);
                  if (!board->undo_move ())
                  {
                     cerr << "Cannot undo while in QS move generation" << endl;
                     abort ();
                  }
               }
               else if (error == Board::DRAW_BY_REPETITION)
               {
                  if (!board->undo_move ())
                  {
                     cerr << "Cannot undo while in QS move generation" << endl;
                     cerr << "Error: draw by repetition" << endl;
                     abort ();
                  }
               }
               else if (error != Board::KING_LEFT_IN_CHECK)
               {
                  cerr << "An error of type " << error
                       << " detected in quiescence move generation" << endl;
               }
            }
         }
      }
   }

   // Sort captures by Most-Valuable-Victim / Least-Valuable-Attacker ratio
   sort (captures.begin (), captures.end ());

   // Add captures
   for (u_int i = 0, n = captures.size (); i < n; ++i)
      moves.push_back (captures[i]);

   // Add checks and check evasions
   for (u_int i = 0, n = checks.size (); i < n; ++i)
      moves.push_back (checks[i]);

   for (u_int i = 0, n = check_evasions.size (); i < n; ++i)
      moves.push_back (check_evasions[i]);

   // Add pawn promotions
   for (u_int i = 0, n = promotions.size (); i < n; ++i)
      moves.push_back (promotions[i]);

   // Add the rest of the moves
   for (u_int i = 0, n = other_moves.size (); i < n; ++i)
      moves.push_back (other_moves[i]);

   return moves.size () != 0;
}

bool
SimpleMoveGenerator::generate_en_prise_evations (Board* board, 
                                                 vector<Move>& moves)
{
   Piece::Player player = board->get_turn ();
   //Piece::Player rival  = (player == Piece::WHITE ? Piece::BLACK : Piece::WHITE);

   bitboard pieces = board->get_pieces (player);
   while (pieces)
   {
      Board::Squares from = Board::Squares (Util::LSB_position (pieces));
      Piece::Type piece_type = board->get_piece (from);
      bitboard threats = board->threats_to (from, piece_type, false);

      // If there are threats, there are three ways to evade them: (a) by moving
      // your own piece, (b) by placing a lower-value piece between your piece
      // and the attacker, and (c) by capturing the attacker (but this last one
      // is not handled here since Quiescence Search does that already)
      if (threats)
      {
         // (a) Move our own piece
         bitboard evasions = board->get_moves (piece_type, from);
         while (evasions)
         {
            Board::Squares to = Board::Squares (Util::LSB_position (evasions));
            Move move (from, to);
            moves.push_back (move);
            // Watch out! removing a bit this way only works for the LSB
            evasions &= (evasions - 1);
         }

         // (b) Place a lower-piece in-between (doesn't work for Knights)
         // if (threats & board->get_pieces (rival, Piece::BISHOP))
         // ... more code will come here!
      }
      // Watch out! removing a bit this way only works for the LSB
      pieces &= (pieces - 1);
   }

   return moves.size () != 0;
}
