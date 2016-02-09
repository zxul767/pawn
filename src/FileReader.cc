/*==============================================================================
   Class name: FileReader

   Responsabilities:
   -Load and save games to disk using an ad hoc protocol.

   Future improvements: Use the PGN protocol to write and read games.
 ==============================================================================*/

#include "FileReader.h"
#include <cctype>

using std::string;
using std::vector;

FileReader:: FileReader ()
{
   this->values = new vector<string>[3];
}

FileReader:: ~FileReader ()
{
   delete[] this->values;
}

FileReader::FileReader (const string& file_name, Board* board)
{
   this->values = new vector<string>[3];
   this->file_name = file_name;
   this->board = board;

   load_variables ();
   load_values ();
}

bool
FileReader::load_variables ()
{
   this->variables.push_back ("GAME_STATUS");

   this->variables.push_back ("TURN");

   this->variables.push_back ("CASTLE_WHITE_KING");
   this->variables.push_back ("CASTLE_WHITE_QUEEN");
   this->variables.push_back ("CASTLE_BLACK_KING");
   this->variables.push_back ("CASTLE_BLACK_QUEEN");

   this->variables.push_back ("EN_PASSANT");

   this->variables.push_back ("WHITE_PAWN");
   this->variables.push_back ("WHITE_KNIGHT");
   this->variables.push_back ("WHITE_BISHOP");
   this->variables.push_back ("WHITE_ROOK");
   this->variables.push_back ("WHITE_QUEEN");
   this->variables.push_back ("WHITE_KING");

   this->variables.push_back ("BLACK_PAWN");
   this->variables.push_back ("BLACK_KNIGHT");
   this->variables.push_back ("BLACK_BISHOP");
   this->variables.push_back ("BLACK_ROOK");
   this->variables.push_back ("BLACK_QUEEN");
   this->variables.push_back ("BLACK_KING");

   return true;
}

bool
FileReader::load_values ()
{
   this->values[0].push_back ("OVER");
   this->values[0].push_back ("PENDING");

   this->values[1].push_back ("WHITE");
   this->values[1].push_back ("BLACK");

   this->values[2].push_back ("TRUE");
   this->values[2].push_back ("FALSE");

   return true;
}

bool
FileReader::load_file ()
{
   input.open (this->file_name.c_str ());

   if (!input.good ())
      return false;

   return true;
}

bool
FileReader::is_empty_line (const string& line) const
{
   if (line.empty ())
      return true;

   for (uint i = 0; i < line.length (); ++i)
      if (!isspace (line[i]))
        return false;

   return true;
}

bool
FileReader::set_variables ()
{
   char c;
   string line;

   if (!load_file ())
      return false;

   while (!input.eof ())
   {
      input >> c;
      if (c != '#')
      {
         input.putback (c);
         getline (input, line);

         if (!is_empty_line (line))
            if (!set_variable (line))
               return false;
      }
      else
        getline (input, line);
   }
   input.close ();

   return true;
}

void
FileReader::tokenize (
    const string& input, vector<string>& tokens, const string& delimiters) const
{
   string::size_type begin = input.find_first_not_of (delimiters, 0);
   string::size_type end = input.find_first_of (delimiters, begin);

   while (begin != string::npos || end != string::npos)
   {
      tokens.push_back (input.substr (begin, end - begin));
      begin = input.find_first_not_of (delimiters, end);
      end = input.find_first_of (delimiters, begin);
   }
}

bool
FileReader::set_variable (string& input)
{
   uint index_variable, index_value;
   vector<string> tokens;
   string delimiters = " :,";

   tokenize (input, tokens, delimiters);
   if (tokens.size () > 1)
   {
      if (!variable_was_found (tokens[0], &index_variable))
        return false;

      for (uint i = 1; i < tokens.size (); ++i)
        if (value_was_found (index_variable, tokens[i], &index_value))
          set_value (index_variable, tokens[i], index_value);
        else
          return false;
   }
   return true;
}

bool
FileReader::variable_was_found (const string& input, uint* index_variable)
{
   for (uint i = 0; i < this->variables.size (); i++)
      if (input == this->variables[i])
      {
         *index_variable = i;
         return true;
      }
   return false;
}

bool
FileReader::value_was_found (
    uint index_variable, const string& input, uint* index_value)
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

bool
FileReader::set_value (
    uint index_variable, const string& token, uint index_value)
{
   if (index_variable == 0)
   {
      this->board->set_game_status (get_status (index_value));
   }
   else if (index_variable == 1)
   {
      this->board->set_turn (get_turn (index_value));
   }
   else if (index_variable >= 2 && index_variable <= 5)
   {
      this->board->set_castling_privilege (
          get_color(index_variable),
          get_side (index_variable),
          get_boolean (index_value));
   }
   else if (index_variable == 6)
   {
      // TODO: implement feature
      //this->board->set_en_passant (get_boolean (index_value));
   }
   else if (index_variable >= 7 && index_variable <= 18)
   {
      this->board->add_piece (
          token,
          get_type (index_variable),
          get_color (index_variable));
   }
   return false;
}

Board::GameStatus
FileReader::get_status (uint index_value) const
{
   return (index_value == 0? Board:: GAME_OVER: Board::PENDING_GAME);
}

Piece::Player
FileReader::get_turn (uint index_value) const
{
   return (index_value == 0? Piece:: WHITE: Piece:: BLACK);
}

bool
FileReader::get_boolean (uint index_value) const
{
   return (index_value == 0? true: false);
}

Piece::Player
FileReader::get_color (uint variable) const
{
   if (variable == 2 || variable == 3 || (variable >= 7 && variable <= 12))
      return Piece:: WHITE;

   return Piece:: BLACK;
}

Board::CastleSide
FileReader::get_side (uint variable) const
{
   if (variable == 3 || variable == 5)
      return Board::QUEEN_SIDE;

   return Board::KING_SIDE;
}

Piece::Type
FileReader::get_type (uint variable) const
{
   if (variable == 7 || variable == 13)
      return Piece:: PAWN;
   if (variable == 8 || variable == 14)
      return Piece:: KNIGHT;
   if (variable == 9 || variable == 15)
      return Piece:: BISHOP;
   if (variable == 10 || variable == 16)
      return Piece:: ROOK;
   if (variable == 3 || variable == 5 || variable == 11 || variable == 17)
      return Piece:: QUEEN;

   return Piece:: KING;
}
