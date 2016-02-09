/*==============================================================================
    Class name: CommandExecuter

    Responsabilities:
    => Carry out the commands provided by the user (a list of available commands
    can be found in Command.h)
  ==============================================================================*/

#include "CommandExecuter.h"
#include "Command.h"
#include "AlphaBetaSearch.h"
#include "Board.h"
#include "SimpleMoveGenerator.h"
#include "Timer.h"
#include "FitnessEvaluator.h"
#include "GeneticAlgorithm.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <memory>

using std::string;
using std::vector;
using std::cout;
using std::cerr;

CommandExecuter::CommandExecuter (
    Board* board, Search* search_engine, Timer* timer)
{
   this->timer = timer;
   this->board = board;
   this->search_engine = search_engine;
   this->move_generator = new SimpleMoveGenerator ();
}

bool
CommandExecuter::execute (const Command& command)
{
   switch (command.get_value ())
   {
   case Command::NEW_GAME:
      this->board->reset ();
      break;

   case Command::UNDO_MOVE:
      this->board->undo_move ();
      break;

   case Command::REMOVE:
      this->board->undo_move ();
      this->board->undo_move ();
      break;

   case Command::SEE_MOVES:
      show_possible_moves ();
      break;

   case Command::USER_MOVE:
      make_user_move (command.get_notation ());
      break;

   case Command::FEATURES:
      cout << "feature setboard=1 usermove=1 time=0 draw=0 sigint=0 "
           << "sigterm=0 variants=\"normal\" analyze=0 colors=0 "
           << "myname=\"MaE\" done=1" << std::endl;
      break;

   case Command::XBOARD_MODE:
      cout << std::endl;
      break;

   case Command::THINK:
      think ();
      break;

   case Command::TRAIN:
      train_by_GA (6, 15, 0.004);
      break;

   default:
      break;
   }
   return true;
}

/*==============================================================================
    Show the set of all valid moves starting from the current configuration of
    THIS->BOARD.
  ==============================================================================*/
void
CommandExecuter::show_possible_moves ()
{
   vector<Move> possible_moves;

   this->move_generator->generate_moves (board, possible_moves);
   cerr << possible_moves.size () << " moves" << std::endl;

   for (uint i = 0; i < possible_moves.size (); ++i)
      if (this->board->make_move (possible_moves[i], true) == Board::NO_ERROR)
      {
         cerr << possible_moves[i] << " -> " << possible_moves[i].get_score ()
                   << std::endl;

         cerr << (*board) << std::endl;

         this->timer->set_clock (2.0);
         this->timer->sleep ();

         this->board->undo_move ();
      }
}

/*==============================================================================
    Communicate to the engine a move made by the user in the GUI
  ==============================================================================*/
void
CommandExecuter::make_user_move (const string& command)
{
   string::size_type i = command.find (" ");

   if (i != string::npos)
   {
      // Strip off the string 'usermove ' sent by Xboard before the actual move
      string notation = command.substr (i + 1);
      Move move (notation);
      Board::Error error = this->board->make_move (move, false);

      switch (error)
      {
      case Board::NO_ERROR:
         think ();
         break;

      case Board::DRAW_BY_REPETITION:
         cout << "1/2-1/2 {Draw by repetition}" << std::endl;
         break;

      case Board::WRONG_MOVEMENT:
         cout << "Illegal move: " << notation;
         break;

      case Board::OPPONENTS_TURN:
         cout << "Illegal move: " << notation;
         break;

      default:
         break;
      }
   }
}

/*==============================================================================
    Query the engine for the most promising move and communicate the response
    to the GUI. It also communicates check mates and draws.
  ==============================================================================*/
void
CommandExecuter::think ()
{
   Search::Result result;
   Move best_move;
   uint depth = 3;

   result = this->search_engine->get_best_move (depth, board, best_move);

   if (result == Search::NORMAL_EVALUATION ||
       result == Search::BLACK_MATES ||
       result == Search::WHITE_MATES)
   {
      Board::Error error = this->board->make_move (best_move, true);
      if (error == Board::NO_ERROR)
      {
         string first, second;
         Move::translate_to_notation (best_move.from (), first);
         Move::translate_to_notation (best_move.to (), second);

         // Communicate the move to Xboard
         cout << "move " << first + second << std::endl;
      }
      else
      {
         // These errors are not supposed to happen
         switch (error)
         {
         case Board::KING_LEFT_IN_CHECK:
            break;

         case Board::OPPONENTS_TURN:
            break;

         case Board::WRONG_MOVEMENT:
            break;

         case Board::GAME_FINISHED:
            break;

         case Board::NO_PIECE_IN_SQUARE:
            break;

         case Board::DRAW_BY_REPETITION:
            cout << "1/2-1/2 {Draw by repetition}" << std::endl;
            break;

         default:
            abort ();
            break;
         }
      }

      if (result == Search::WHITE_MATES)
      {
         cout << "1-0 {White mates}" << std::endl;
      }
      else if (result == Search::BLACK_MATES)
      {
         cout << "0-1 {Black mates}" << std::endl;
      }
   }
   else if (result == Search::STALEMATE)
   {
      cout << "1/2-1/2 {Stalemate}" << std::endl;
   }
   else if (result == Search::DRAW_BY_REPETITION)
   {
      cout << "1/2-1/2 {Draw by repetition}" << std::endl;
   }
}

void
CommandExecuter::train_by_GA (
    uint population_size, uint n_generations, double mutation_probability)
{
   std::unique_ptr<FitnessEvaluator> fitness_evaluator(
       new FitnessEvaluator (this->search_engine));

   std::unique_ptr<GeneticAlgorithm> GA(
       new GeneticAlgorithm (
           population_size, n_generations, mutation_probability, fitness_evaluator.get()));

   vector<int> features_a;
   features_a.push_back (80);
   features_a.push_back (10);
   features_a.push_back (20);
   features_a.push_back (10);
   Chromosome seed (features_a);

   GA->set_seed (seed);
   GA->run ();

   vector<int> best_features;
   GA->get_fittest_member ().decode (best_features);

   this->search_engine->load_factor_weights (best_features);
}
