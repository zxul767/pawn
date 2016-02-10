#ifndef COMMAND_EXECUTER_H
#define COMMAND_EXECUTER_H

#include <string>
#include "Util.hpp"

class Command;
class Board;
class Timer;
class IChessEngine;
class MoveGenerator;

class CommandExecuter
{
 public:
   CommandExecuter (Board* board, IChessEngine* chess_engine, Timer* timer);

   bool execute (const Command& command);
   void show_possible_moves ();
   void make_user_move (const std::string& command);
   void think ();
   void train_by_GA (
       uint population_size, uint n_generations, double mutation_probability);

 private:
   Timer* timer;
   Board* board;
   IChessEngine* chess_engine;
   MoveGenerator* move_generator;
};

#endif // COMMAND_EXECUTER_H
