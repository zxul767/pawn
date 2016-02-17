#ifndef COMMAND_READER_H
#define COMMAND_READER_H

#include "UserCommand.hpp"

namespace game_ui
{
class UserCommandReader
{
  public:
   UserCommandReader ();

   UserCommand get_user_command ();
};

} // namespace game_ui

#endif // COMMAND_READER_H
