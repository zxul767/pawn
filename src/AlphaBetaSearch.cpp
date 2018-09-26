/*==============================================================================
  To improve the performance of the basic alpha-beta search algorithm, some
  other algorithms and techniques are used on top of it (e.g. transposition
  tables, iterative deepening search, null-window search, etc.)
  ==============================================================================*/

#include "IBoard.hpp"
#include "AlphaBetaSearch.hpp"
#include "MoveGenerator.hpp"
#include "PositionEvaluator.hpp"
#include "TranspositionTable.hpp"
#include "BoardKey.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>

namespace game_engine
{
using std::vector;
using game_rules::Move;
using game_rules::IBoard;

AlphaBetaSearch::AlphaBetaSearch (
    IPositionEvaluator* position_evaluator, IMoveGenerator* move_generator)
{
   this->board = nullptr;
   this->move_generator = move_generator;
   this->position_evaluator = position_evaluator;
   this->transposition_table = new TranspositionTable (64);
}

AlphaBetaSearch::~AlphaBetaSearch ()
{
   delete this->transposition_table;
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
IEngine::GameResult
AlphaBetaSearch::get_best_move (uint depth, IBoard* board, Move& best_move)
{
   GameResult winner[game_rules::PLAYERS_COUNT][game_rules::PLAYERS_COUNT] = {
      { GameResult::WHITE_MATES, GameResult::BLACK_MATES },
      { GameResult::BLACK_MATES, GameResult::WHITE_MATES }
   };

   if (board == 0)
      return IEngine::ERROR;

   this->board = board;
   this->max_depth = depth;

   vector<Move> principal_variation;
   this->root_value = iterative_deepening (principal_variation);

   if (abs (this->root_value) == abs(MATE_VALUE))
      this->result = winner[root_value > 0 ? 0 : 1][board->get_player_in_turn ()];

   else if (root_value == DRAW_VALUE && result == STALEMATE)
      this->result = GameResult::STALEMATE;

   else
      this->result = GameResult::NORMAL_EVALUATION;

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
   uint search_window_size = pow(2, 6);
   int alpha, beta;

   // This estimation of the negamax value may be really wrong if we are in
   // the middle of a tactical sequence
   this->root_value = this->position_evaluator->static_evaluation (board);

   for (uint depth = 1; depth <= this->max_depth; ++depth)
   {
      this->max_depth = depth;

      // Search for the right negamax value by using reduced alpha-beta windows
      while (1)
      {
         reset_statistics ();

         // Close window around the likely real value of the root node.
         alpha = root_value - search_window_size;
         beta = root_value + search_window_size;

         this->root_value = alpha_beta (0, alpha, beta);

         if (abs (this->root_value) == abs(MATE_VALUE))
            break;

         if (this->root_value > alpha && this->root_value < beta)
         {
            search_window_size /= 2;
            break;
         }
         search_window_size *= 2;
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

   this->result = GameResult::NORMAL_EVALUATION;

   // Probe the transposition table to avoid recomputing
   bool hash_hit = false;
   TranspositionTable::BoardEntry entry;
   BoardKey key = {
      this->board->get_hash_key (),
      this->board->get_hash_lock ()
   };
   if (this->transposition_table->get_entry (key, entry))
   {
      if (entry.depth >= max_depth - depth)
         if (entry.accuracy == TranspositionTable::EXACT ||
             (entry.accuracy == TranspositionTable::UPPER_BOUND && entry.score >= beta) ||
             (entry.accuracy == TranspositionTable::LOWER_BOUND && entry.score <= alpha))
         {
            if (this->board->get_repetition_count () == 1)
            {
               this->hash_table_hits++;
               this->n_leaf_nodes++;
               this->best_move = entry.best_move;

               return entry.score;
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
      return util::Util::max (
          this->position_evaluator->static_evaluation (this->board),
          quiescence (0, alpha, beta));
   }

   this->move_generator->generate_moves (this->board, moves);
   if (moves.size () == 0)
   {
      if (!this->board->is_king_in_check ())
      {
         this->n_nodes_evaluated++;
         return this->position_evaluator->static_evaluation (this->board);
      }
      return MATE_VALUE;
   }

   // Improve move ordering by examining the principal_variation node at this ply
   // found in the previous iteration or maybe in a search previously done with
   // a narrower alpha-beta window
   if (hash_hit)
   {
      vector<Move>::iterator p = find (moves.begin (), moves.end (), entry.best_move);
      if (p != moves.end ())
      {
         moves.erase (p);
         moves.insert (moves.begin (), entry.best_move);
      }
   }

   uint n_moves_made = 0;
   for (uint i = 0, n = moves.size (); i < n; ++i)
   {
      IBoard::Error error = this->board->make_move (moves[i], /* is_computer_move: */ true);
      if (error == IBoard::KING_LEFT_IN_CHECK)
         continue;

      n_moves_made++;

      if (error == IBoard::DRAW_BY_REPETITION)
      {
         tentative_value = DRAW_VALUE;
      }
      else
      {
         assert(error == IBoard::NO_ERROR);
         tentative_value = -alpha_beta (depth + 1, -beta, -util::Util::max (alpha, best_value));
      }

      assert(this->board->undo_move ());

      if (tentative_value > best_value)
      {
         if (error == IBoard::DRAW_BY_REPETITION)
            this->result = DRAW_BY_REPETITION;
         else
            this->result = NORMAL_EVALUATION;

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
      if (this->board->is_king_in_check ())
         best_value = MATE_VALUE;
      else
      {
         std::cerr << "Draw by stalemate" << std::endl;
         this->result = STALEMATE;
         best_value = DRAW_VALUE;
      }
   }
   else
   {
      // Store this board configuration along with its negamax value or the
      // lower/upper bound found. Also store the depth to which it was explored.
      uint real_depth = max_depth - depth;
      TranspositionTable::flag accuracy =
            best_value >= beta ? TranspositionTable::UPPER_BOUND :
            best_value > alpha ? TranspositionTable::EXACT :
            TranspositionTable::LOWER_BOUND;

      this->transposition_table->add_entry (
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
   node_value = this->position_evaluator->static_evaluation (board);

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
   if (depth >= MAX_QUIESCENCE_DEPTH && !this->board->is_king_in_check ())
   {
      this->n_leaf_nodes++;

      return node_value;
   }
   this->move_generator->generate_moves (
       this->board, moves,
       IMoveGenerator::CAPTURES |
       IMoveGenerator::CHECKS   |
       IMoveGenerator::CHECK_EVASIONS |
       IMoveGenerator::PAWN_PROMOTIONS);

   // A checkmate
   if (this->board->is_king_in_check () && moves.size () == 0)
   {
      this->n_leaf_nodes++;

      return MATE_VALUE;
   }

   // An apparently quiescent position
   else if (moves.size () == 0)
   {
      // Make sure none of the pieces of the player in turn is being
      // attacked by a lower value piece
      this->move_generator->generate_en_prise_evations (this->board, moves);

      if (moves.size () == 0)
      {
         this->n_leaf_nodes++;

         return node_value;
      }
   }

   this->n_internal_nodes++;
   for (uint i = 0, n = moves.size (); i < n; ++i)
   {
      IBoard::Error error = this->board->make_move (moves[i], /* is_computer_move: */ true);
      if (error == IBoard::KING_LEFT_IN_CHECK)
         continue;

      if (error == IBoard::DRAW_BY_REPETITION)
         tentative_value = DRAW_VALUE;
      else
         tentative_value = -quiescence (depth + 1, -beta, -alpha);

      this->average_branching_factor++;

      assert(this->board->undo_move ());

      if (tentative_value > best_value)
      {
         if (error == IBoard::DRAW_BY_REPETITION)
            this->result = GameResult::DRAW_BY_REPETITION;
         else
            this->result = GameResult::NORMAL_EVALUATION;

         best_value = tentative_value;
         if (best_value >= beta) // Alpha-beta cutoff
            break;
      }
   }

   // If all the possible violent moves (captures, checks, pawn promotions, ...)
   // are bad, we are not forced to make any move, unless we are in check
   if (best_value < node_value && !this->board->is_king_in_check ())
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
    IBoard* board, vector<Move>& principal_variation)
{
   BoardKey key = {
      board->get_hash_key (),
      board->get_hash_lock ()
   };
   TranspositionTable::BoardEntry entry;
   bool return_value = true;

   if (this->transposition_table->get_entry (key, entry))
   {
      principal_variation.push_back (entry.best_move);
      IBoard::Error error = board->make_move (entry.best_move, /* is_computer_move: */ true);

      if (error == IBoard::NO_ERROR)
      {
         if (!build_principal_variation (board, principal_variation))
         {
            principal_variation.pop_back ();
            return_value = false;
         }
         assert(board->undo_move ());
      }
      else if (error != IBoard::KING_LEFT_IN_CHECK &&
               error != IBoard::DRAW_BY_REPETITION)
      {
         assert(board->undo_move ());
         return_value = false;
      }
      else if (error == IBoard::DRAW_BY_REPETITION)
      {
         assert(board->undo_move ());
      }
   }

   return return_value;
}

void
AlphaBetaSearch::load_factor_weights (vector<int>& weights)
{
   this->transposition_table->reset ();
   this->position_evaluator->load_factor_weights (weights);
}

} // namespace game_engine
