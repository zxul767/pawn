#include <iostream>
#include <string>
#include <memory>

#include "MaeBoard.hpp"
#include "Timer.hpp"
#include "UserCommandReader.hpp"
#include "UserCommandExecuter.hpp"
#include "AlphaBetaSearch.hpp"
#include "MoveGenerator.hpp"
#include "PositionEvaluator.hpp"

using std::unique_ptr;
using std::cerr;
using std::endl;

using game_rules::IBoard;
using game_rules::MaeBoard;
using game_rules::Piece;
using game_rules::Move;

using game_engine::IMoveGenerator;
using game_engine::MoveGenerator;
using game_engine::IPositionEvaluator;
using game_engine::PositionEvaluator;
using game_engine::IEngine;
using game_engine::AlphaBetaSearch;

using game_ui::UserCommand;
using game_ui::UserCommandReader;
using game_ui::UserCommandExecuter;

using diagnostics::Timer;

int
main ()
{
  bool auto_play = true;
  bool xboard_mode = false;

  unique_ptr<IBoard> board(new MaeBoard());
  unique_ptr<IPositionEvaluator> position_evaluator(new PositionEvaluator());
  unique_ptr<IMoveGenerator> generator(new MoveGenerator());
  unique_ptr<IEngine> search_engine(
      new AlphaBetaSearch (position_evaluator.get(), generator.get()));

  unique_ptr<Timer> timer(new Timer);

  UserCommand command;
  unique_ptr<UserCommandReader> command_reader(new UserCommandReader());
  unique_ptr<UserCommandExecuter> command_executer(
      new UserCommandExecuter(board.get(), search_engine.get(), timer.get()));

  cerr << (*board) << endl;
  cerr << (board->get_player_in_turn () == Piece::WHITE ?
           "[White's turn]: " : "[Black's turn]: ");

  command = command_reader->get_user_command ();
  while (!command.is_quit ())
  {
    if (command.is_move ())
    {
      Move move (command.get_notation ());

      IBoard::Error error = board->make_move (move, false);
      if (error != IBoard::NO_ERROR)
      {
        switch (error)
        {
          case IBoard::GAME_FINISHED:
            cerr << "Game finished" << endl;
            break;
          case IBoard::NO_PIECE_IN_SQUARE:
            cerr << "No piece in square" << endl;
            break;
          case IBoard::OPPONENTS_TURN:
            cerr << "Opponents turn" << endl;
            break;
          case IBoard::WRONG_MOVEMENT:
            cerr << "Wrong movement" << endl;
            break;
          case IBoard::KING_LEFT_IN_CHECK:
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
        UserCommand response (UserCommand::THINK);
        command_executer->execute (response);
      }
    }
    else
    {
      if (command.get_key () == UserCommand::XBOARD_MODE)
      {
        xboard_mode = true;
        auto_play = true;
        command_executer->execute (command);
      }
      else if (command.get_key () == UserCommand::COMPUTER_PLAY)
      {
        auto_play = !auto_play;
        if (auto_play)
        {
          UserCommand response (UserCommand::THINK);
          command_executer->execute (response);
        }
      }
      else
        command_executer->execute (command);
    }

    if (!xboard_mode)
    {
      cerr << (*board) << endl;
      cerr << (board->get_player_in_turn () == Piece::WHITE ?
               "[White's turn]: " : "[Black's turn]: ");
    }

    command = command_reader->get_user_command ();
  }

  return 0;
}
