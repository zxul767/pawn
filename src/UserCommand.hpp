#ifndef USER_COMMAND_H
#define USER_COMMAND_H

/*==============================================================================
  Represents a command issued by the user, providing the information
  associated with it (e.g. a command to move a piece should also provide the
  start and end squares)
  ==============================================================================*/

#include <map>
#include <string>

namespace game_ui
{
using std::string;

class UserCommand
{
  public:
    enum CommandKey
    {
        XBOARD_MODE,
        FEATURES,
        NEW_GAME,
        QUIT,
        THINK,
        UNDO_MOVE,
        SEE_MOVES,
        USER_MOVE,
        REMOVE,
        MOVE,
        TRAIN,
        COMPUTER_PLAY,
        UNKNOWN
    };

    UserCommand();
    UserCommand(CommandKey key);
    UserCommand(string &notation);

    bool is_move() const;
    bool is_quit() const;

    string get_notation() const;
    CommandKey get_key() const;

  private:
    string notation;
    CommandKey key;

    static std::map<string, CommandKey> notation_to_key;
    static std::map<CommandKey, string> key_to_notation;

    static bool load_commands();
    static const bool commands_loaded;
};

} // namespace game_ui

#endif // USER_COMMAND_H
