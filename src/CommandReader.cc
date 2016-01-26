/*==============================================================================
   Class name: CommandReader

   Responsabilities:
   - Read commands from the terminal (either from the user or from a GUI that
   communicates using standard I/O).
 ==============================================================================*/

#include "CommandReader.h"
#include "Command.h"

#include <string>
#include <iostream>

CommandReader::CommandReader ()
{
   // Remove the buffer from the output stream
   std::cout.setf (std::ios::unitbuf);
}

Command
CommandReader::get_command ()
{
   std::string command = "";

   std::getline (std::cin, command);

   return Command (command);
}
