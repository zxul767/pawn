#ifndef COMMAND_READER_H
#define COMMAND_READER_H

#include "Command.h"

class CommandReader
{
 public:
   CommandReader ();

   Command get_command ();
};

#endif
