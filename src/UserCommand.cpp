#include "UserCommand.hpp"
#include "Move.hpp"

namespace game_ui
{
using std::string;
using game_rules::Move;

std::map<string, UserCommand::CommandKey> UserCommand::notation_to_key;
std::map<UserCommand::CommandKey, string> UserCommand::key_to_notation;

const bool UserCommand::commands_loaded = UserCommand::load_commands ();

UserCommand::UserCommand ()
{
   this->key = UNKNOWN;
   this->notation = "";
}

UserCommand::UserCommand (CommandKey key)
{
   this->key = key;
   this->notation = UserCommand::key_to_notation[key];
}

UserCommand::UserCommand (string& notation)
{
   this->notation = notation;

   if (UserCommand::notation_to_key.find (notation) != UserCommand::notation_to_key.end ())
      this->key = UserCommand::notation_to_key[notation];

   else if (notation.find ("usermove") != string::npos &&
            notation.find ("accepted") == string::npos)
   {
      this->key = USER_MOVE;
   }
   else if (!Move (notation).is_null ()) {
      this->key = MOVE;
   }
   else {
      this->key = UNKNOWN;
   }
}

bool
UserCommand::load_commands ()
{
   notation_to_key["xboard"] = XBOARD_MODE;
   notation_to_key["protover 2"] = FEATURES;
   notation_to_key["new"] = NEW_GAME;
   notation_to_key["quit"] = QUIT;
   notation_to_key["think"] = THINK;
   notation_to_key["undo"] = UNDO_MOVE;
   notation_to_key["see moves"] = SEE_MOVES;
   notation_to_key["usermove"] = USER_MOVE;
   notation_to_key["remove"] = REMOVE;
   notation_to_key["train"] = TRAIN;
   notation_to_key["auto"] = COMPUTER_PLAY;

   key_to_notation[XBOARD_MODE] = "xboard";
   key_to_notation[FEATURES] = "protover 2";
   key_to_notation[NEW_GAME] = "new";
   key_to_notation[QUIT] = "quit";
   key_to_notation[THINK] = "think";
   key_to_notation[UNDO_MOVE] = "undo";
   key_to_notation[SEE_MOVES] = "see moves";
   key_to_notation[USER_MOVE] = "usermove";
   key_to_notation[REMOVE] = "remove";
   key_to_notation[TRAIN] = "train";
   key_to_notation[COMPUTER_PLAY] = "auto";

   return true;
}

bool
UserCommand::is_quit () const
{
   return this->key == QUIT;
}

bool
UserCommand::is_move () const
{
   return this->key == MOVE;
}

UserCommand::CommandKey
UserCommand::get_key () const
{
   return this->key;
}

string
UserCommand::get_notation () const
{
   return this->notation;
}

} // namespace game_ui
