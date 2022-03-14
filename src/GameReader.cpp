#include "GameReader.hpp"
#include <cctype>

namespace game_persistence
{
using std::string;
using std::vector;

GameReader::GameReader()
{
    this->values = new vector<string>[3];
}

GameReader::~GameReader()
{
    delete[] this->values;
}

GameReader::GameReader(const string &file_name, IBoard *board)
{
    this->values = new vector<string>[3];
    this->file_name = file_name;
    this->board = board;

    load_variables();
    load_values();
}

bool GameReader::load_variables()
{
    this->variables.push_back("GAME_STATUS");

    this->variables.push_back("TURN");

    this->variables.push_back("CASTLE_WHITE_KING");
    this->variables.push_back("CASTLE_WHITE_QUEEN");
    this->variables.push_back("CASTLE_BLACK_KING");
    this->variables.push_back("CASTLE_BLACK_QUEEN");

    this->variables.push_back("EN_PASSANT");

    this->variables.push_back("WHITE_PAWN");
    this->variables.push_back("WHITE_KNIGHT");
    this->variables.push_back("WHITE_BISHOP");
    this->variables.push_back("WHITE_ROOK");
    this->variables.push_back("WHITE_QUEEN");
    this->variables.push_back("WHITE_KING");

    this->variables.push_back("BLACK_PAWN");
    this->variables.push_back("BLACK_KNIGHT");
    this->variables.push_back("BLACK_BISHOP");
    this->variables.push_back("BLACK_ROOK");
    this->variables.push_back("BLACK_QUEEN");
    this->variables.push_back("BLACK_KING");

    return true;
}

bool GameReader::load_values()
{
    this->values[0].push_back("OVER");
    this->values[0].push_back("PENDING");

    this->values[1].push_back("WHITE");
    this->values[1].push_back("BLACK");

    this->values[2].push_back("TRUE");
    this->values[2].push_back("FALSE");

    return true;
}

bool GameReader::load_file()
{
    input.open(this->file_name.c_str());

    if (!input.good())
        return false;

    return true;
}

bool GameReader::is_empty_line(const string &line) const
{
    if (line.empty())
        return true;

    for (uint i = 0; i < line.length(); ++i)
        if (!isspace(line[i]))
            return false;

    return true;
}

bool GameReader::set_variables()
{
    char c;
    string line;

    if (!load_file())
        return false;

    while (!input.eof())
    {
        input >> c;
        if (c != '#')
        {
            input.putback(c);
            getline(input, line);

            if (!is_empty_line(line))
                if (!set_variable(line))
                    return false;
        }
        else
            getline(input, line);
    }
    input.close();

    return true;
}

void GameReader::tokenize(const string &input, vector<string> &tokens, const string &delimiters) const
{
    string::size_type begin = input.find_first_not_of(delimiters, 0);
    string::size_type end = input.find_first_of(delimiters, begin);

    while (begin != string::npos || end != string::npos)
    {
        tokens.push_back(input.substr(begin, end - begin));
        begin = input.find_first_not_of(delimiters, end);
        end = input.find_first_of(delimiters, begin);
    }
}

bool GameReader::set_variable(string &input)
{
    uint index_variable, index_value;
    vector<string> tokens;
    string delimiters = " :,";

    tokenize(input, tokens, delimiters);
    if (tokens.size() > 1)
    {
        if (!variable_was_found(tokens[0], &index_variable))
            return false;

        for (uint i = 1; i < tokens.size(); ++i)
            if (value_was_found(index_variable, tokens[i], &index_value))
                set_value(index_variable, tokens[i], index_value);
            else
                return false;
    }
    return true;
}

bool GameReader::variable_was_found(const string &input, uint *index_variable)
{
    for (uint i = 0; i < this->variables.size(); i++)
        if (input == this->variables[i])
        {
            *index_variable = i;
            return true;
        }
    return false;
}

bool GameReader::value_was_found(uint index_variable, const string &input, uint *index_value)
{
    int index = index_variable;

    if (index_variable >= 7 && index_variable <= 18)
        return true;

    if (index_variable >= 2 && index_variable <= 6)
        index = 2;

    for (uint i = 0; i < 2; i++)
        if (input == this->values[index][i])
        {
            *index_value = i;
            return true;
        }
    return false;
}

bool GameReader::set_value(uint index_variable, const string &token, uint index_value)
{
    if (index_variable == 0)
    {
        this->board->set_game_status(get_status(index_value));
    }
    else if (index_variable == 1)
    {
        this->board->set_player_in_turn(get_player_in_turn(index_value));
    }
    else if (index_variable >= 2 && index_variable <= 5)
    {
        this->board->set_castling_privilege(get_color(index_variable), get_side(index_variable),
                                            get_boolean(index_value));
    }
    else if (index_variable == 6)
    {
        // TODO: implement loading en-passant capture possibility
        // this->board->set_en_passant_capture_square (get_boolean (index_value));
    }
    else if (index_variable >= 7 && index_variable <= 18)
    {
        this->board->add_piece(token, get_type(index_variable), get_color(index_variable));
    }
    return false;
}

IBoard::GameStatus GameReader::get_status(uint index_value) const
{
    return (index_value == 0 ? IBoard::GAME_OVER : IBoard::PENDING_GAME);
}

Piece::Player GameReader::get_player_in_turn(uint index_value) const
{
    return (index_value == 0 ? Piece::WHITE : Piece::BLACK);
}

bool GameReader::get_boolean(uint index_value) const
{
    return (index_value == 0 ? true : false);
}

Piece::Player GameReader::get_color(uint variable) const
{
    if (variable == 2 || variable == 3 || (variable >= 7 && variable <= 12))
        return Piece::WHITE;

    return Piece::BLACK;
}

game_rules::CastleSide GameReader::get_side(uint variable) const
{
    if (variable == 3 || variable == 5)
        return game_rules::CastleSide::QUEEN_SIDE;

    return game_rules::CastleSide::KING_SIDE;
}

Piece::Type GameReader::get_type(uint variable) const
{
    if (variable == 7 || variable == 13)
        return Piece::PAWN;
    if (variable == 8 || variable == 14)
        return Piece::KNIGHT;
    if (variable == 9 || variable == 15)
        return Piece::BISHOP;
    if (variable == 10 || variable == 16)
        return Piece::ROOK;
    if (variable == 3 || variable == 5 || variable == 11 || variable == 17)
        return Piece::QUEEN;

    return Piece::KING;
}

} // namespace game_persistence
