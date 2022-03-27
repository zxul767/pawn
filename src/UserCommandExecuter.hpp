#ifndef USER_COMMAND_EXECUTER_H
#define USER_COMMAND_EXECUTER_H

#include <string>

namespace rules
{
class IBoard;
}

namespace engine
{
class IEngine;
class IMoveGenerator;
} // namespace engine

namespace diagnostics
{
class Timer;
}

namespace game_ui
{
using std::string;

class UserCommand;

class UserCommandExecuter
{
  public:
    UserCommandExecuter(rules::IBoard *, engine::IEngine *, diagnostics::Timer *);

    bool execute(const UserCommand &);
    void show_possible_moves();
    void make_user_move(const string &command);
    void think();
    void train_by_genetic_algorithm(
        uint population_size, uint generations_count, double mutation_probability);

  private:
    diagnostics::Timer *timer;
    rules::IBoard *board;
    engine::IEngine *engine;
    engine::IMoveGenerator *move_generator;
};

} // namespace game_ui

#endif // USER_COMMAND_EXECUTER_H
