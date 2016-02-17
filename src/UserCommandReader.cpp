#include "UserCommandReader.hpp"
#include "UserCommand.hpp"

#include <string>
#include <iostream>

namespace game_ui
{
UserCommandReader::UserCommandReader ()
{
   // Remove the buffer from the output stream
   std::cout.setf (std::ios::unitbuf);
}

UserCommand
UserCommandReader::get_user_command ()
{
   std::string command = "";

   std::getline (std::cin, command);

   return UserCommand (command);
}

} // namespace game_ui
