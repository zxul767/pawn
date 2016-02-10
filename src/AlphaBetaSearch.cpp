/*==============================================================================
  To improve the performance of the basic alpha-beta search algorithm, some
  other algorithms and techniques are used on top of it (e.g. transposition
  tables, iterative deepening search, null-window search, etc.)
 ==============================================================================*/

#include "Board.hpp"
#include "AlphaBetaSearch.hpp"
#include "MoveGenerator.hpp"
#include "PositionEvaluator.hpp"
#include "Dictionary.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>

using std::vector;

AlphaBetaSearch::AlphaBetaSearch (
    PositionEvaluator* evaluator, MoveGenerator* generator)
{
   this->board = nullptr;
   this->generator = generator;
   this->evaluator = evaluator;
   this->hash_table = new Dictionary (64);
}

AlphaBetaSearch::~AlphaBetaSearch ()
{
   delete this->hash_table;
}

void
AlphaBetaSearch::print_statistics (vector<Move>& principal_variation)
{
   if (this->n_internal_nodes)
   {
      this->average_branching_factor =
            (uint)
            ((double)(this->average_branching_factor / this->n_internal_nodes) + 0.5);
   }

   std::cerr << "Evaluation: " << this->root_value << std::endl;
   std::cerr << "Average branching factor: "
             << this->average_branching_factor << std::endl;
   std::cerr << "Leaf nodes: " << this->n_leaf_nodes << std::endl;
   std::cerr << "Nodes evaluated: " << this->n_nodes_evaluated << std::endl;
   std::cerr << "Hash table hits: " << this->hash_table_hits << std::endl;

   std::cerr << "Principal variation: " << std::endl;
   for (uint i = 0; i < principal_variation.size (); ++i)
      std::cerr << principal_variation[i] << std::endl;
}

void
AlphaBetaSearch::reset_statistics ()
{
   this->n_nodes_evaluated = 0;
   this->n_internal_nodes = 0;
   this->n_leaf_nodes = 0;
   this->average_branching_factor = 0;
   this->hash_table_hits = 0;
}

/*==============================================================================
  Return in BEST_MOVE the most promising move that can be made in the current
  BOARD, doing a search of DEPTH levels.

  Possible results are: NORMAL_EVALUATION, WHITE_MATES, BLACK_MATES,
  STALEMATE, DRAW_BY_REPETITION.
  ==============================================================================*/
IChessEngine::Result
AlphaBetaSearch::get_best_move (uint depth, Board* board, Move& best_move)
{
   Result winner[Board::PLAYERS][Board::PLAYERS] = {
      { WHITE_MATES, BLACK_MATES },
      { BLACK_MATES, WHITE_MATES }
   };

   if (board == 0)
      return IChessEngine::ERROR;

   this->board = board;
   this->max_depth = depth;

   vector<Move> principal_variation;
   this->root_value = iterative_deepening (principal_variation);

   if (abs (this->root_value) == abs(MATE_VALUE))
      this->result = winner[root_value > 0 ? 0 : 1][board->get_turn ()];

   else if (root_value == DRAW_VALUE && result == STALEMATE)
      this->result = STALEMATE;

   else
      this->result = NORMAL_EVALUATION;

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
   uint expanding_offset = pow(2, 6);
   int alpha, beta;

   // This estimation of the negamax value may be really wrong if we are in
   // the middle of a tactical sequence
   this->root_value = this->evaluator->static_evaluation (board);

   for (uint depth = 1; depth <= this->max_depth; ++depth)
   {
      this->max_depth = depth;

      // Search for the right negamax value by using reduced alpha-beta windows
      while (1)
      {
         reset_statistics ();

         // Close window around the likely real value of the root node.
         alpha = root_value - expanding_offset;
         beta = root_value + expanding_offset;

         this->root_value = alpha_beta (0, alpha, beta);

         if (abs (this->root_value) == abs(MATE_VALUE))
            break;

         if (this->root_value > alpha && this->root_value < beta)
         {
            expanding_offset /= 2;
            break;
         }
         expanding_offset *= 2;
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
   ushort best_value_index = 0;
   int tentative_value;
   int best_value = MATE_VALUE; // Initially the best_value you can do is lose the game!

   result = NORMAL_EVALUATION;

   // Probe the transposition table to avoid recomputing
   bool hash_hit = false;
   Dictionary::hash_info data;
   Dictionary::board_key key = { board->get_hash_key (), board->get_hash_lock ()};
   if (this->hash_table->get_data (key, data))
   {
      if (data.depth >= max_depth - depth)
         if (data.accuracy == Dictionary::EXACT ||
             (data.accuracy == Dictionary::UPPER_BOUND && data.score >= beta) ||
             (data.accuracy == Dictionary::LOWER_BOUND && data.score <= alpha))
         {
            if (board->get_repetition_count () == 1)
            {
               this->hash_table_hits++;
               this->n_leaf_nodes++;
               this->best_move = data.best_move;

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
      this->n_nodes_evaluated++;
      return Util::max (
          this->evaluator->static_evaluation (board),
          quiescence (0, alpha, beta));
   }

   this->generator->generate_moves (board, moves);
   if (moves.size () == 0)
   {
      if (!board->is_king_in_check ())
      {
         this->n_nodes_evaluated++;
         return this->evaluator->static_evaluation (board);
      }
      return MATE_VALUE;
   }

   // Improve move ordering by examining the principal_variation node at this ply
   // found in the previous iteration or maybe in a search previously done with
   // a narrower alpha-beta window
   if (hash_hit)
   {
      vector<Move>::iterator p = find (moves.begin (), moves.end (), data.best_move);
      if (p != moves.end ())
      {
         moves.erase (p);
         moves.insert (moves.begin (), data.best_move);
      }
   }

   uint n_moves_made = 0;
   for (uint i = 0, n = moves.size (); i < n; ++i)
   {
      Board::Error error = board->make_move (moves[i], true);
      if (error == Board::KING_LEFT_IN_CHECK)
         continue;

      n_moves_made++;

      if (error == Board::DRAW_BY_REPETITION)
      {
         tentative_value = DRAW_VALUE;
      }
      else
      {
         assert(error == Board::NO_ERROR);
         tentative_value = -alpha_beta (depth + 1, -beta, -Util::max (alpha, best_value));
      }

      assert(board->undo_move ());

      if (tentative_value > best_value)
      {
         if (error == Board::DRAW_BY_REPETITION)
            result = DRAW_BY_REPETITION;
         else
            result = NORMAL_EVALUATION;

         best_value = tentative_value;
         best_value_index = i;
         if (best_value >= beta) // Alpha-beta cutoff
            break;
      }
   }

   this->average_branching_factor += n_moves_made;

   // The king must be in mate or stalemate since no move was made
   if (n_moves_made == 0)
   {
      std::cerr << "No moves could be done" << std::endl;
      if (board->is_king_in_check ())
         best_value = MATE_VALUE;
      else
      {
         std::cerr << "Draw by stalemate" << std::endl;
         result = STALEMATE;
         best_value = DRAW_VALUE;
      }
   }
   else
   {
      // Store this board configuration along with its negamax value or the
      // lower/upper bound found. Also store the depth to which it was explored.
      uint real_depth = max_depth - depth;
      Dictionary::flag accuracy;

      if (best_value >= beta)       accuracy = Dictionary::UPPER_BOUND;
      else if (best_value > alpha)  accuracy = Dictionary::EXACT;
      else                          accuracy = Dictionary::LOWER_BOUND;

      this->hash_table->add_entry (
          key, best_value, accuracy, moves[best_value_index], real_depth);
      this->best_move = moves[best_value_index];
      this->n_internal_nodes++;
   }

   return best_value;
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
   int best_value = MATE_VALUE;

   this->n_nodes_evaluated++;
   node_value = this->evaluator->static_evaluation (board);

   // Assumption made: making a move will improve the position
   // In zugzwang positions, this is not true.
   if (node_value >= beta)
   {
      this->n_leaf_nodes++;

      return node_value;
   }

   // Tighten the alpha-beta window
   if (node_value > alpha)
      alpha = node_value;

   // Failing to pay attention to an ongoing check has fatal consequences
   if (depth >= MAX_QUIESCENCE_DEPTH && !board->is_king_in_check ())
   {
      this->n_leaf_nodes++;

      return node_value;
   }
   this->generator->generate_moves (
       board, moves,
       MoveGenerator::CAPTURES |
       MoveGenerator::CHECKS   |
       MoveGenerator::CHECK_EVASIONS |
       MoveGenerator::PAWN_PROMOTIONS);

   // A checkmate
   if (board->is_king_in_check () && moves.size () == 0)
   {
      this->n_leaf_nodes++;

      return MATE_VALUE;
   }

   // An apparently quiescent position
   else if (moves.size () == 0)
   {
      // Make sure none of the pieces of the player in turn is being
      // attacked by a lower value piece
      this->generator->generate_en_prise_evations (board, moves);

      if (moves.size () == 0)
      {
         this->n_leaf_nodes++;

         return node_value;
      }
   }

   this->n_internal_nodes++;
   for (uint i = 0, n = moves.size (); i < n; ++i)
   {
      Board::Error error = board->make_move (moves[i], true);
      if (error == Board::KING_LEFT_IN_CHECK)
         continue;

      if (error == Board::DRAW_BY_REPETITION)
         tentative_value = DRAW_VALUE;
      else
         tentative_value = -quiescence (depth + 1, -beta, -alpha);

      this->average_branching_factor++;

      assert(board->undo_move ());

      if (tentative_value > best_value)
      {
         if (error == Board::DRAW_BY_REPETITION)
            result = IChessEngine::DRAW_BY_REPETITION;
         else
            result = NORMAL_EVALUATION;

         best_value = tentative_value;
         if (best_value >= beta) // Alpha-beta cutoff
            break;
      }
   }

   // If all the possible violent moves (captures, checks, pawn promotions, ...)
   // are bad, we are not forced to make any move, unless we are in check
   if (best_value < node_value && !board->is_king_in_check ())
      best_value = node_value;

   return best_value;
}

/*============================================================================
  Build the principal variation rooted at BOARD and until a leaf is reached

  Return true if there was no problem building the principal variation, and
  false otherwise --all errors detected here are serious bugs, so watch out!
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

   if (this->hash_table->get_data (key, data))
   {
      principal_variation.push_back (data.best_move);
      Board::Error error = board->make_move (data.best_move, true);

      if (error == Board::NO_ERROR)
      {
         if (!build_principal_variation (board, principal_variation))
         {
            std::cerr << "couldn't build rest of path" << std::endl;
            principal_variation.pop_back ();
            return_value = false;
         }
         assert(board->undo_move ());
      }
      else if (error != Board::KING_LEFT_IN_CHECK &&
               error != Board::DRAW_BY_REPETITION)
      {
         assert(board->undo_move ());
         return_value = false;
      }
      else if (error == Board::DRAW_BY_REPETITION)
      {
         assert(board->undo_move ());
      }
   }

   return return_value;
}

void
AlphaBetaSearch::load_factor_weights (vector<int>& weights)
{
   this->hash_table->reset ();
   this->evaluator->load_factor_weights (weights);
}
