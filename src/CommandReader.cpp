#include "CommandReader.hpp"
#include "Command.hpp"

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
