#ifndef COMMAND_EXECUTER_H
#define COMMAND_EXECUTER_H

#include <string>
#include "Util.hpp"

namespace game_rules { class Board; }
namespace game_engine { class IChessEngine; class MoveGenerator; }
namespace diagnostics { class Timer; }

namespace game_ui
{
class Command;

class CommandExecuter
{
  public:
   CommandExecuter (game_rules::Board*, game_engine::IChessEngine*, diagnostics::Timer*);

   bool execute (const Command&);
   void show_possible_moves ();
   void make_user_move (const std::string& command);
   void think ();
   void train_by_GA (
       uint population_size, uint n_generations, double mutation_probability);

  private:
   diagnostics::Timer* timer;
   game_rules::Board* board;
   game_engine::IChessEngine* chess_engine;
   game_engine::MoveGenerator* move_generator;
};

} // game_ui

#endif // COMMAND_EXECUTER_H
