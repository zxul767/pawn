#include "Command.hpp"
#include "Move.hpp"

namespace game_ui
{
using std::string;
using game_rules::Move;

std::map<string, Command::Value> Command::commands;
std::map<Command::Value, string> Command::commandStrings;

const bool Command::commands_loaded = Command::load_commands ();

Command::Command ()
{
   this->value = UNKNOWN;
   this->command = "";
}

Command::Command (Value command_value)
{
   this->value = command_value;
   this->command = commandStrings[value];
}

Command::Command (string& command)
{
   this->command = command;

   if (Command::commands.find (command) != Command::commands.end ())
      this->value = commands[command];

   else if (command.find ("usermove") != string::npos &&
            command.find ("accepted") == string::npos)
   {
      this->value = USER_MOVE;
   }
   else if (!Move (command).is_null ()) {
      this->value = MOVE;
   }
   else {
      this->value = UNKNOWN;
   }
}

bool
Command::load_commands ()
{
   commands["xboard"] = XBOARD_MODE;
   commands["protover 2"] = FEATURES;
   commands["new"] = NEW_GAME;
   commands["quit"] = QUIT;
   commands["think"] = THINK;
   commands["undo"] = UNDO_MOVE;
   commands["see moves"] = SEE_MOVES;
   commands["usermove"] = USER_MOVE;
   commands["remove"] = REMOVE;
   commands["train"] = TRAIN;
   commands["auto"] = COMPUTER_PLAY;

   commandStrings[XBOARD_MODE] = "xboard";
   commandStrings[FEATURES] = "protover 2";
   commandStrings[NEW_GAME] = "new";
   commandStrings[QUIT] = "quit";
   commandStrings[THINK] = "think";
   commandStrings[UNDO_MOVE] = "undo";
   commandStrings[SEE_MOVES] = "see moves";
   commandStrings[USER_MOVE] = "usermove";
   commandStrings[REMOVE] = "remove";
   commandStrings[TRAIN] = "train";
   commandStrings[COMPUTER_PLAY] = "auto";

   return true;
}

bool
Command::is_quit () const
{
   return this->value == QUIT;
}

bool
Command::is_move () const
{
   return this->value == MOVE;
}

Command::Value
Command::get_value () const
{
   return this->value;
}

string
Command::get_notation () const
{
   return this->command;
}

} // namespace game_ui
