#include <iostream>
#include <string>

#include "MaeBoard.h"
#include "Timer.h"
#include "CommandReader.h"
#include "CommandExecuter.h"
#include "AlphaBetaSearch.h"
#include "MinimaxSearch.h"
#include "SimpleMoveGenerator.h"
#include "SimpleEvaluator.h"

using std::cout;
using std::cerr;
using std::endl;

int
main ()
{
   bool               auto_play = false;
   bool               xboard_mode = false;
   Board*             board = new MaeBoard ();
   PositionEvaluator* evaluator = new SimpleEvaluator ();
   MoveGenerator*     generator = new SimpleMoveGenerator ();
   Search*            search_engine = new AlphaBetaSearch (evaluator, generator);
   Timer*             timer = new Timer ();

   Command            command;
   CommandReader*     command_reader = new CommandReader ();
   CommandExecuter*   command_executer = 
      new CommandExecuter(board, search_engine, timer);

   /* MAIN LOOP OF THE APPLICATION */
   cerr << (*board) << endl;
   cerr << (board->get_turn () == Piece::WHITE ?
            "[White's turn]: " : "[Black's turn]: ");
  
   command = command_reader->get_command ();
   while (!command.is_quit ())
   {
      if (command.is_move ())
      {
         Move move (command.get_notation ());

         Board::Error error = board->make_move (move, false);
         if (error != Board::NO_ERROR)
         {
            switch (error)
            {
               case Board::GAME_FINISHED:
                  cerr << "Game finished" << endl;
                  break;
               case Board::NO_PIECE_IN_SQUARE:
                  cerr << "No piece in square" << endl;
                  break;
               case Board::OPPONENTS_TURN:
                  cerr << "Opponents turn" << endl;
                  break;
               case Board::WRONG_MOVEMENT:
                  cerr << "Wrong movement" << endl;
                  break;
               case Board::KING_LEFT_IN_CHECK:
                  cerr << "King left in check" << endl;
                  break;
               default:
                  cerr << "Another error" << endl;
                  break;
            }
            cerr << "[Your move is illegal, please try again.]" 
                 << endl;
         }
         else if (auto_play)
         {
            Command response (Command::THINK);
            command_executer->execute (response);
         }
      }
      else
      {
         if (command.get_value () == Command::XBOARD_MODE)
         {
            xboard_mode = true;
            auto_play = true;
            command_executer->execute (command);
         }
         else if (command.get_value () == Command::COMPUTER_PLAY)
         {
            auto_play = !auto_play;
            if (auto_play)
            {
               Command response (Command::THINK);
               command_executer->execute (response);
            }
         }
         else
            command_executer->execute (command);
      }

      if (!xboard_mode)
      {
         cerr << (*board) << endl;
         cerr << (board->get_turn () == Piece::WHITE ?
                  "[White's turn]: " : "[Black's turn]: ");
      }

      command = command_reader->get_command ();
   }

   delete board;   
   delete search_engine;
   delete evaluator;
   delete generator;
   delete timer;
   delete command_reader;
   delete command_executer;

   return 0;
}
