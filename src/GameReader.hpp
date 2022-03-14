#ifndef FILE_READER_H
#define FILE_READER_H

/*==============================================================================
  Reads chess games in an ad-hoc notation (see initial.in)
  ==============================================================================*/

#include <fstream>
#include <string>
#include <vector>

#include "IBoard.hpp"
#include "Piece.hpp"

namespace game_persistence
{
using game_rules::IBoard;
using game_rules::Piece;

class GameReader
{
  public:
    GameReader();
    GameReader(const std::string &filename, game_rules::IBoard *board);
    ~GameReader();

    bool set_variables();

  private:
    game_rules::IBoard *board;
    std::string file_name;
    std::ifstream input;
    std::vector<std::string> variables;
    std::vector<std::string> *values;

    bool load_variables();
    bool load_values();
    bool load_file();

    bool is_empty_line(const std::string &line) const;
    bool set_variable(std::string &input);

    bool variable_was_found(const std::string &input, uint *index_variable);
    bool value_was_found(uint index_variable, const std::string &input, uint *index_value);

    bool set_value(uint index_variable, const std::string &token, uint index_value);
    bool get_boolean(uint index_value) const;

    void tokenize(const std::string &input, std::vector<std::string> &tokens,
                  const std::string &delimiters = " ") const;

    game_rules::IBoard::GameStatus get_status(uint index_value) const;
    game_rules::Piece::Player get_player_in_turn(uint index_value) const;
    game_rules::Piece::Player get_color(uint variable) const;
    game_rules::CastleSide get_side(uint variable) const;
    game_rules::Piece::Type get_type(uint variable) const;
};

} // namespace game_persistence

#endif // FILE_READER_H
