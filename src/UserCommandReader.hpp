#ifndef COMMAND_READER_H
#define COMMAND_READER_H

namespace game_ui
{
class UserCommand;

class UserCommandReader
{
  public:
    UserCommandReader();

    UserCommand get_user_command();
};

} // namespace game_ui

#endif // COMMAND_READER_H
