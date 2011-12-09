#include "FitnessEvaluator.h"
#include "SimpleEvaluator.h"
#include "Search.h"
#include "Chromosome.h"
#include "MaeBoard.h"
#include "Move.h"
#include "Timer.h"

using std::cerr;
using std::endl;

#include <cstdlib>

FitnessEvaluator::FitnessEvaluator (Search* engine)
{
   this->engine = engine;
   board = new MaeBoard ();
   evaluator = new SimpleEvaluator ();
}

FitnessEvaluator::~FitnessEvaluator ()
{
   delete board;
   delete evaluator;
}

// Chromosome A is assumed to be stronger than B, though this may prove to be
// wrong after the game is played...
double
FitnessEvaluator::evaluate (Chromosome& a, Chromosome& b)
{
   double          evaluation = 0.0;
   vector<int>     features[2];

   a.decode (features[0]);
   b.decode (features[1]);

   /**************************************************************************
     Set up a game between the two and see who wins and by how much...
     Use a single engine, just load and unload the factor weights after
     every move...
   ***************************************************************************/
   Timer* timer = new Timer ();
   timer->start ();

   board->reset ();
   cerr << (*board) << endl;
   //std::cin.get ();

   // Chromosome A plays white
   Search::Result result;
   uint           turn = 0;
   do
   {
      engine->load_factor_weights (features[turn]);

      cerr << (*board) << endl;
      bitboard key = board->get_hash_key ();
      bitboard lock = board->get_hash_lock ();

      Move move;
      result = engine->get_best_move (3, board, move);
      if (result == Search::WHITE_MATES || 
          result == Search::BLACK_MATES ||
          result == Search::STALEMATE) break;

      if (key != board->get_hash_key () || lock != board->get_hash_lock ())
      {
         cerr << "Our board has been apparently corrupted" << endl;
         cerr << (*board);
      }

      cerr << (turn ? "black" : "white") << " ";
      cerr << "moves " << move << endl;

      Board::Error error = board->make_move (move, true);
      if (error != Board::NO_ERROR && error != Board::DRAW_BY_REPETITION)
      {
         cerr << "An unexpected error has occurred: " << error << endl;
         cerr << (*board) << endl;
         abort ();
      }
      else if (error == Board::DRAW_BY_REPETITION)
      {
         board->undo_move ();
         result = Search::DRAW_BY_REPETITION;
         break;
      }
      turn = (turn + 1) % 2;

      if (board->get_move_number() >= MAX_ALLOWED_MOVEMENTS)
      {
         cerr << "too many movements";
         int r = evaluator->evaluate_material(board);

         if (r == 0)
            result = Search::STALEMATE;
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
   cerr << "Time elapsed: " << timer->elapsed_time () 
        << " seconds " << endl;

   // EVALUATE WHO WON AND BY HOW MUCH...
   if (result == Search::STALEMATE || result == Search::DRAW_BY_REPETITION)
   {
      evaluation = 1.0 / 2.0;
      cerr << "Draw" << endl;      
      b.set_result (Chromosome::DRAW);
   }
   else
   {
      evaluation = 6.0 / 9.0;
   
      if (result == Search::WHITE_MATES)
      {
         evaluation = 1 - evaluation;
         cerr << "white mates" << endl;
         b.set_result (Chromosome::LOSS);
      }
      else
      {
         cerr << "black mates" << endl;
         b.set_result (Chromosome::WIN);
      }
      
      b.set_game_duration (board->get_move_number ());
   }

   b.set_material_balance (evaluator->evaluate_material(board));
   b.set_fitness (evaluation);
   //cerr << "Final Position" << endl;
   //cerr << (*board) << endl;
   
   delete timer;

   return evaluation;
}
