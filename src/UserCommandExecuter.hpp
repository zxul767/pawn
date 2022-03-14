#ifndef USER_COMMAND_EXECUTER_H
#define USER_COMMAND_EXECUTER_H

#include "Util.hpp"
#include <string>

namespace game_rules
{
class IBoard;
}
namespace game_engine
{
class IEngine;
class MoveGenerator;
} // namespace game_engine
namespace diagnostics
{
class Timer;
}

namespace game_ui
{
class UserCommand;

class UserCommandExecuter
{
  public:
    UserCommandExecuter(game_rules::IBoard *, game_engine::IEngine *, diagnostics::Timer *);

    bool execute(const UserCommand &);
    void show_possible_moves();
    void make_user_move(const std::string &command);
    void think();
    void train_by_genetic_algorithm(uint population_size, uint generations_count, double mutation_probability);

  private:
    diagnostics::Timer *timer;
    game_rules::IBoard *board;
    game_engine::IEngine *game_engine;
    game_engine::MoveGenerator *move_generator;
};

} // namespace game_ui

#endif // USER_COMMAND_EXECUTER_H
