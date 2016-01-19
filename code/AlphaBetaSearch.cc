/*============================================================================
   Class name: AlphaBetaSearch

   Responsabilities:
   -Implement the Search interface.

   Techniques used to improve performance of the basic Negamax algorithm:

   => Fail-soft alpha-beta pruning
   => Iterative deepening search (IDS)
   => Quiescence Search
   => Aspiration search integrated within the IDS
   => Transposition tables
 ============================================================================*/
#include "Board.h"
#include "AlphaBetaSearch.h"
#include "MoveGenerator.h"
#include "PositionEvaluator.h"
#include "Dictionary.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstdlib>

using std::cerr;
using std::cout;
using std::endl;

AlphaBetaSearch::AlphaBetaSearch (PositionEvaluator* evaluator,
                                  MoveGenerator* generator)
{
   board            = 0;
   this->generator  = generator;
   this->evaluator  = evaluator;
   this->hash_table = new Dictionary (64);

   debugging = false;
}

AlphaBetaSearch::~AlphaBetaSearch ()
{
   delete hash_table;
}

void
AlphaBetaSearch::print_statistics (vector<Move>& principal_variation)
{
   if (n_internal_nodes)
   {
      average_branching_factor = (uint)
         ((double)(average_branching_factor / n_internal_nodes) + 0.5);
   }

   cerr << "--------------------------------------------------" << endl;
   cerr << "Evaluation: " << root_value << endl;
   cerr << "Average branching factor: " << average_branching_factor << endl;
   cerr << "Leaf nodes: " << n_leaf_nodes << endl;
   cerr << "Nodes evaluated: " << n_nodes_evaluated << endl;
   cerr << "Hash table hits: " << hash_table_hits << endl;
   cerr << "Principal variation: " << endl;
   for (uint i = 0; i < principal_variation.size (); ++i)
      cout << principal_variation[i] << endl;
   cout << "--------------------------------------------------" << endl;
}

void
AlphaBetaSearch::reset_statistics ()
{
   n_nodes_evaluated = 0;
   n_internal_nodes = 0;
   n_leaf_nodes = 0;
   average_branching_factor = 0;
   hash_table_hits = 0;
}

/*==============================================================================
   Return in BEST_MOVE the most promising move that can be made in the current
   BOARD, doing a search of DEPTH levels.

   Possible results are: NORMAL_EVALUATION, WHITE_MATES, BLACK_MATES,
   STALEMATE, DRAW_BY_REPETITION.
 ==============================================================================*/
Search::Result
AlphaBetaSearch::get_best_move (uint depth, Board* board, Move& best_move)
{
   Result winner[Board::PLAYERS][Board::PLAYERS] = {
     { WHITE_MATES, BLACK_MATES },
     { BLACK_MATES, WHITE_MATES }
   };

   if (board == 0)
      return Search::ERROR;

   this->board     = board;
   this->max_depth = depth;

   vector<Move> principal_variation;
   root_value = iterative_deepening (principal_variation);

   if (abs (root_value) == abs(MATE_VALUE))
      result = winner[root_value > 0 ? 0 : 1][board->get_turn ()];

   else if (root_value == DRAW_VALUE && result == STALEMATE)
      result = STALEMATE;

   else
      result = NORMAL_EVALUATION;

   // If we are not using transposition tables, we cannot reconstruct the
   // principal variation
   if (principal_variation.size () > 0)
      best_move = principal_variation[0];
   else
      best_move = this->best_move;

   return this->result;
}

/*==========================================================================
   Perform an iterative deepening search using THIS->BOARD as the root
   node. Include Aspiration Search within the main loop to increase the
   overall performance. Transposition tables help here very much in cases
   where a re-search is needed (i.e. the value returned by alpha-beta
   outside the alpha-beta windows)

   Return the minimax value of THIS->BOARD and the principal variation in
   PRINCIPAL_VARIATION.
 ==========================================================================*/
int
AlphaBetaSearch::iterative_deepening (vector<Move>& principal_variation)
{
   uint max_ply = this->max_depth;
   uint expanding_offset = 1 << 6;
   int alpha, beta;

   // This estimation of the negamax value may be really wrong if we are in
   // the middle of a tactical sequence
   root_value = evaluator->static_evaluation (board);
   this->turn_debugging (false);

   for (uint depth = 1; depth <= max_ply; ++depth)
   {
      //cerr << "--------------------------------------------------" << endl;
      //cerr << "ITERATION #" << depth << endl;

      this->max_depth = depth;

      // Search for the right negamax value by using reduced alpha-beta windows
      while (1)
      {
         reset_statistics ();

         // Close window around the likely real value of the root node.
         alpha = root_value - expanding_offset;
         beta  = root_value + expanding_offset;

         root_value = alpha_beta (0, alpha, beta);

         if (abs (root_value) == abs(MATE_VALUE))
            break;

         if (root_value > alpha && root_value < beta)
         {
            expanding_offset >>= 1;
            break;
         }
         expanding_offset <<= 1;
      }
   }

   principal_variation.clear ();
   build_principal_variation (board, principal_variation);

   // Ensure that the code still works even if transposition tables are
   // removed
   if (principal_variation.size () == 0)
      principal_variation.push_back (best_move);

   return root_value;
}

/*==============================================================================
   Perform a minimax search with alpha-beta pruning, evaluating all lines of
   play to level DEPTH, and continuing with Quiescence search at the leaf
   nodes

   Return the minimax value of the node represented by the current board in
   THIS->BOARD. Note that this value is positive if the player in turn at the
   root node has the advantage, and negative if not.
 ==============================================================================*/
int
AlphaBetaSearch::alpha_beta (uint depth, int alpha, int beta)
{
   vector<Move> moves;
   ushort best_index = 0;
   int tentative_value;
   int best = MATE_VALUE; // Initially the best you can do is lose the game!

   result = NORMAL_EVALUATION;

   // Probe the transposition table to avoid recomputing
   bool hash_hit = false;
   Dictionary::hash_info data;
   Dictionary::board_key key = { board->get_hash_key (), board->get_hash_lock ()};
   if (hash_table->get_data (key, data))
   {
      if (data.depth >= max_depth - depth)
         if (data.accuracy == Dictionary::EXACT ||
             (data.accuracy == Dictionary::UPPER_BOUND && data.score >= beta) ||
             (data.accuracy == Dictionary::LOWER_BOUND && data.score <= alpha))
         {
            if (board->get_repetition_count () == 1)
            {
               hash_table_hits++;
               n_leaf_nodes++;
               best_move = data.best;
               return data.score;
            }
         }
      hash_hit = true;
   }

   // BASE CASE
   if (depth >= max_depth)
   {
      // Quiescence search may return a value that is well below the current
      // node evaluation, meaning that all captures considered are really bad.
      ++n_nodes_evaluated;
      int node_value = evaluator->static_evaluation (board);
      int quiescence_value = quiescence (0, alpha, beta);
      return Util::max (node_value, quiescence_value);
   }

   generator->generate_moves (board, moves);
   if (moves.size () == 0)
   {
      if (!board->is_king_in_check ())
      {
         ++n_nodes_evaluated;
         return evaluator->static_evaluation (board);
      }
      return MATE_VALUE;
   }

   // Improve move ordering by examining the principal_variation node at this ply
   // found in the previous iteration or maybe in a search previously done with
   // a narrower alpha-beta window
   if (hash_hit)
   {
      vector<Move>::iterator p = find (moves.begin (), moves.end (), data.best);
      if (p != moves.end ())
      {
         moves.erase (p);
         moves.insert (moves.begin (), data.best);
      }
   }

   uint n_moves_made = 0;
   for (uint i = 0, n = moves.size (); i < n; ++i)
   {
      Board::Error error = board->make_move (moves[i], true);
      if (error == Board::KING_LEFT_IN_CHECK)
         continue;

      ++n_moves_made;

      if (error == Board::DRAW_BY_REPETITION)
      {
         tentative_value = DRAW_VALUE;
      }
      else if (error == Board::NO_ERROR)
         tentative_value =
            -alpha_beta (depth + 1, -beta, -Util::max (alpha, best));
      else
      {
         cerr << "fatal error returned by board during search: "
              << error << " when making move " << moves[i] << endl;
         cerr << (*board) << endl;

         abort ();
      }

      if (!board->undo_move ())
      {
         cerr << "fatal error: couldn't undo." << endl;
         abort ();
      }

      if (tentative_value > best)
      {
         if (error == Board::DRAW_BY_REPETITION)
            result = DRAW_BY_REPETITION;
         else
            result = NORMAL_EVALUATION;

         best = tentative_value;
         best_index = i;
         if (best >= beta) // Alpha-beta cutoff
            break;
      }
   }

   average_branching_factor += n_moves_made;

   // The king must be in mate or stalemate since no move was made
   if (n_moves_made == 0)
   {
      cerr << "No moves could be done" << endl;
      if (board->is_king_in_check ())
         best = MATE_VALUE;
      else
      {
         cerr << "Draw by stalemate" << endl;
         result = STALEMATE;
         best = DRAW_VALUE;
      }
   }
   else
   {
      // Store this board configuration along with its negamax value or the
      // lower/upper bound found. Also store the depth to which it was explored.
      uint real_depth = max_depth - depth;
      Dictionary::flag accuracy;
      if (best >= beta)       accuracy = Dictionary::UPPER_BOUND;
      else if (best > alpha)  accuracy = Dictionary::EXACT;
      else                    accuracy = Dictionary::LOWER_BOUND;

      hash_table->add_entry (key, best, accuracy, moves[best_index], real_depth);
      best_move = moves[best_index];
      n_internal_nodes++;
   }

   return best;
}

/*============================================================================
   Perform a quiescence search taking into account only lines of captures.

   Return the score of the best line of play within the horizon established
   by MAX_QUIESCENCE_DEPTH.
 ============================================================================*/
int
AlphaBetaSearch::quiescence (uint depth, int alpha, int beta)
{
   vector<Move> moves;
   int tentative_value, node_value;
   int best = MATE_VALUE;

   ++n_nodes_evaluated;
   node_value = evaluator->static_evaluation (board);

   // Assumption made: making a move will improve the position
   // In zugzwang positions, this is not true.
   if (node_value >= beta)
   {
      if (debugging)
      {
         cerr << (board->get_turn () == Piece::WHITE ? "WHITE":"BLACK") << endl;
         cerr << "Static value of node exceeds BETA: "
              << node_value << " >= " << beta << endl;
         cerr << (*board) << endl;
         std::cin.get ();
      }
      ++n_leaf_nodes;
      return node_value;
   }

   // Tighten the alpha-beta window
   if (node_value > alpha)
      alpha = node_value;

   // Failing to pay attention to an ongoing check has fatal consequences
   if (depth >= MAX_QUIESCENCE_DEPTH && !board->is_king_in_check ())
   {
      if (debugging)
      {
         cerr << (board->get_turn () == Piece::WHITE ? "WHITE":"BLACK") << endl;
         cerr << "MAX_QUIESCENCE_DEPTH exceeded but not in check: "
              << node_value << endl;
         cerr << (*board) << endl;
         std::cin.get ();
      }
      ++n_leaf_nodes;
      return node_value;
   }
   generator->generate_moves (board, moves,
                              MoveGenerator::CAPTURES |
                              MoveGenerator::CHECKS   |
                              MoveGenerator::CHECK_EVASIONS |
                              MoveGenerator::PAWN_PROMOTIONS);

   // A checkmate
   if (board->is_king_in_check () && moves.size () == 0)
   {
      if (debugging)
      {
         cerr << (board->get_turn () == Piece::WHITE ? "WHITE":"BLACK") << endl;
         cerr << "A checkmate" << endl;
         cerr << (*board) << endl;
         std::cin.get ();
      }
      ++n_leaf_nodes;
      return MATE_VALUE;
   }

   // An apparently quiescent position
   else if (moves.size () == 0)
   {
      // Make sure none of the pieces of the player in turn is being
      // attacked by a lower value piece
      generator->generate_en_prise_evations (board, moves);

      if (moves.size () == 0)
      {
         if (debugging)
         {
            cerr << (board->get_turn () == Piece::WHITE ? "WHITE":"BLACK") << endl;
            cerr << "Quiescent position: " << node_value << endl;
            cerr << (*board) << endl;
            std::cin.get ();
         }
         ++n_leaf_nodes;
         return node_value;
      }
   }

   n_internal_nodes++;
   for (uint i = 0, n = moves.size (); i < n; ++i)
   {
      Board::Error error = board->make_move (moves[i], true);
      if (error == Board::KING_LEFT_IN_CHECK)
         continue;

      if (error == Board::DRAW_BY_REPETITION)
         tentative_value = DRAW_VALUE;
      else
         tentative_value = -quiescence (depth + 1, -beta, -alpha);

      average_branching_factor++;

      if (!board->undo_move ())
      {
         cerr << "fatal error: couldn't undo in QS" << endl;
         abort ();
      }

      if (tentative_value > best)
      {
         if (error == Board::DRAW_BY_REPETITION)
            result = Search::DRAW_BY_REPETITION;
         else
            result = NORMAL_EVALUATION;

         best = tentative_value;
         if (best >= beta) // Alpha-beta cutoff
            break;
      }
   }

   // If all the possible violent moves (captures, checks, pawn promotions, ...)
   // are bad, we are not forced to make any move, unless we are in check
   if (best < node_value && !board->is_king_in_check ())
      best = node_value;

   return best;
}

/*============================================================================
   Build the principal variation rooted at BOARD and until a leaf is reached

   Return true if there was no problem building the principal variation, and
   false otherwise -all errors detected here are serious bugs, so watch out!
 ============================================================================*/
bool
AlphaBetaSearch::build_principal_variation (
    Board* board, vector<Move>& principal_variation)
{
   Dictionary::board_key key = {
     board->get_hash_key (),
     board->get_hash_lock ()
   };
   Dictionary::hash_info data;
   bool return_value = true;

   if (debugging)
      cerr << (*board) << endl;

   if (hash_table->get_data (key, data))
   {
      principal_variation.push_back (data.best);
      Board::Error error = board->make_move (data.best, true);

      if (error == Board::NO_ERROR)
      {
         if (!build_principal_variation (board, principal_variation))
         {
            cerr << "couldn't build rest of path" << endl;
            principal_variation.pop_back ();
            return_value = false;
         }
         if (!board->undo_move ())
         {
            cerr << "fatal error: cannot undo" << endl;
            abort ();
         }
      }
      else if (error != Board::KING_LEFT_IN_CHECK &&
               error != Board::DRAW_BY_REPETITION)
      {
         if (!board->undo_move ())
         {
            cerr << "fatal error: cannot undo" << endl;
            abort ();
         }
         cerr << "move not accepted in build path" << error << endl;
         return_value = false;
      }
      else if (error == Board::DRAW_BY_REPETITION)
      {
         if (!board->undo_move ())
         {
            cerr << "fatal error: cannot undo" << endl;
            abort ();
         }
      }
   }
   else if (debugging)
      cerr << "final node" << endl << (*board) << endl;

   return return_value;
}

/*============================================================================
   Load a set of weights for the position evaluator to use. This may result
   in a change of performance, according as to how accurate the weights are
 ============================================================================*/
void
AlphaBetaSearch::load_factor_weights (vector<int>& weights)
{
   hash_table->reset ();
   evaluator->load_factor_weights (weights);
}
