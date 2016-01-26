#include "Board.h"
#include "MinimaxSearch.h"
#include "MoveGenerator.h"
#include "PositionEvaluator.h"

#include <iostream>
#include <cstdlib>

using std::cerr;
using std::cout;
using std::endl;

MinimaxSearch::MinimaxSearch (
    PositionEvaluator* evaluator, MoveGenerator* generator)
{
   board = 0;
   this->generator = generator;
   this->evaluator = evaluator;
}

MinimaxSearch::~MinimaxSearch ()
{
}

Search::Result
MinimaxSearch::get_best_move (
    uint max_depth, Board* board, Move& best_move)
{
   if (board == 0) return Search::ERROR;

   this->board = board;
   n_nodes_evaluated = 0;
   minimax (max_depth, this->result);
   best_move = this->best_move;

   cerr << "Nodes evaluated: " << n_nodes_evaluated << endl;

   return this->result;
}

int
MinimaxSearch::minimax (uint depth, Result& result)
{
   vector<Move>  moves;
   Move best_move;
   int tentative_value, node_value;
   uint n_moves_made;
   Result this_result = NORMAL_EVALUATION;

   generator->generate_moves (board, moves, MoveGenerator::ALL);

   if (moves.size () == 0 || depth == 0)
   {
      n_nodes_evaluated++;
      return evaluator->static_evaluation (board);
   }

   node_value = MATE_VALUE;
   n_moves_made = 0;

   for (uint i = 0, n = moves.size (); i < n; ++i)
   {
      if (board->make_move (moves[i], true) == Board::NO_ERROR)
      {
         n_moves_made++;
         tentative_value = -minimax (depth - 1, this_result);
         board->undo_move ();

         if (tentative_value > node_value)
         {
            node_value = tentative_value;
            best_move = moves[i];
         }
         if (abs (tentative_value) >= Util::INFINITUM)
            best_move = moves[i];
      }
   }

   /* There was no legal move, so the king must be either in check mate 
      or in stalemate
   */
   if (n_moves_made == 0)
   {
      if (!board->is_king_in_check ())
         this_result = STALEMATE;
      else
         this_result = (board->get_turn () == Piece::WHITE)
            ? BLACK_MATES
            : WHITE_MATES;
   }
   else if (depth > 0)
      this_result = NORMAL_EVALUATION;

   this->best_move = best_move;
   result = this_result;

   return (result == STALEMATE ? 0 : node_value);
}

void
MinimaxSearch::load_factor_weights (vector<int>& weights)
{
   cout << "Loading factor weights for minimax" << endl;
}
