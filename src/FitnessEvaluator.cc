#include "FitnessEvaluator.h"
#include "SimpleEvaluator.h"
#include "Search.h"
#include "Chromosome.h"
#include "MaeBoard.h"
#include "Move.h"
#include "Timer.h"

#include <cstdlib>

FitnessEvaluator::FitnessEvaluator (Search* engine)
{
   this->engine = engine;
   this->board = new MaeBoard ();
   this->evaluator = new SimpleEvaluator ();
}

FitnessEvaluator::~FitnessEvaluator ()
{
   delete this->board;
   delete this->evaluator;
}

// Chromosome A is assumed to be stronger than B, though this may prove to be
// wrong after the game is played...
double
FitnessEvaluator::evaluate (Chromosome& a, Chromosome& b)
{
   double evaluation = 0.0;
   std::vector<int> features[2];

   a.decode (features[0]);
   b.decode (features[1]);

   /*--------------------------------------------------------------------------
     Set up a game between the two and see who wins and by how much...
     Use a single engine, just load and unload the factor weights after
     every move...
     -------------------------------------------------------------------------*/
   Timer* timer = new Timer ();
   timer->start ();

   this->board->reset ();

   // Chromosome A plays white
   Search::Result result;
   uint turn = 0;
   do
   {
      this->engine->load_factor_weights (features[turn]);

      // TODO: enable again once we implement the assertion below
      // bitboard key = this->board->get_hash_key ();
      // bitboard lock = this->board->get_hash_lock ();

      Move move;
      result = this->engine->get_best_move (3, this->board, move);
      if (result == Search::WHITE_MATES ||
          result == Search::BLACK_MATES ||
          result == Search::STALEMATE) break;

      // TODO: turn into an assertion
      // if (key == this->board->get_hash_key () || lock != this->board->get_hash_lock ())
      // {
      // }

      Board::Error error = this->board->make_move (move, true);
      if (error != Board::NO_ERROR && error != Board::DRAW_BY_REPETITION)
      {
         abort ();
      }
      else if (error == Board::DRAW_BY_REPETITION)
      {
         this->board->undo_move ();
         result = Search::DRAW_BY_REPETITION;
         break;
      }
      turn = (turn + 1) % 2;

      if (this->board->get_move_number() >= MAX_ALLOWED_MOVEMENTS)
      {
         int r = this->evaluator->evaluate_material(board);

         if (r == 0) {
            result = Search::STALEMATE;
         }
         else if (board->get_turn() == Piece::WHITE)
         {
            if (r > 0)
               result = Search::WHITE_MATES;
            else
               result = Search::BLACK_MATES;
         }
         else
         {
            if (r > 0)
               result = Search::BLACK_MATES;
            else
               result = Search::WHITE_MATES;
         }
         break;
      }
   } while (true);

   timer->stop ();

   // EVALUATE WHO WON AND BY HOW MUCH...
   if (result == Search::STALEMATE || result == Search::DRAW_BY_REPETITION)
   {
      evaluation = 1.0 / 2.0;
      b.set_result (Chromosome::DRAW);
   }
   else
   {
      evaluation = 6.0 / 9.0;

      if (result == Search::WHITE_MATES)
      {
         evaluation = 1 - evaluation;
         b.set_result (Chromosome::LOSS);
      }
      else
      {
         b.set_result (Chromosome::WIN);
      }

      b.set_game_duration (board->get_move_number ());
   }

   b.set_material_balance (this->evaluator->evaluate_material(board));
   b.set_fitness (evaluation);

   delete timer;

   return evaluation;
}
