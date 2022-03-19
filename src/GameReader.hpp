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

namespace serialization
{
using rules::IBoard;
using rules::Piece;
using std::string;
using std::vector;

class GameReader
{
  public:
    GameReader();
    GameReader(const string &filename, rules::IBoard *board);
    ~GameReader();

    bool set_variables();

  private:
    rules::IBoard *board;
    string file_name;
    std::ifstream input;
    vector<string> variables;
    vector<string> *values;

    bool load_variables();
    bool load_values();
    bool load_file();

    bool is_empty_line(const string &line) const;
    bool set_variable(string &input);

    bool variable_was_found(const string &input, uint *index_variable);
    bool value_was_found(uint index_variable, const string &input, uint *index_value);

    bool set_value(uint index_variable, const string &token, uint index_value);
    bool get_boolean(uint index_value) const;

    void tokenize(
        const string &input, vector<string> &tokens,
        const string &delimiters = " ") const;

    rules::IBoard::GameStatus get_status(uint index_value) const;
    rules::Piece::Player current_player(uint index_value) const;
    rules::Piece::Player get_color(uint variable) const;
    rules::CastleSide get_side(uint variable) const;
    rules::Piece::Type get_type(uint variable) const;
};

} // namespace serialization

#endif // FILE_READER_H
