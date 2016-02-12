#ifndef COMMAND_READER_H
#define COMMAND_READER_H

#include "Command.hpp"

namespace game_ui
{
class CommandReader
{
  public:
   CommandReader ();

   Command get_command ();
};

} // namespace game_ui

#endif // COMMAND_READER_H
