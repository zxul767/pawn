#ifndef COMMAND_H
#define COMMAND_H

/*==============================================================================
  Represents a command issued by the user, providing the information
  associated with it (e.g. a command to move a piece should also provide the
  start and end squares)
  ==============================================================================*/

#include <string>
#include <map>

namespace game_ui
{
class Command
{
  public:
   enum Value {
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

   Command ();
   Command (Value command_value);
   Command (std::string& command);

   bool is_move () const;
   bool is_quit () const;

   std::string get_notation () const;
   Value get_value () const;

  private:
   std::string command;
   Value value;

   static std::map<std::string, Value> commands;
   static std::map<Value, std::string> commandStrings;

   static bool load_commands ();
   static const bool commands_loaded;
};

} // namespace game_ui

#endif // COMMAND_H
