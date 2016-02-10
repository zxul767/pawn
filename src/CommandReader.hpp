#ifndef COMMAND_READER_H
#define COMMAND_READER_H

#include "Command.hpp"

class CommandReader
{
 public:
   CommandReader ();

   Command get_command ();
};

#endif
