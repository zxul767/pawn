#ifndef FILE_READER_H
#define FILE_READER_H

#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::ifstream;

#include "Board.h"

class FileReader
{
 public:
   FileReader ();
   FileReader (const string& filename, Board* board);
   ~FileReader ();

   bool set_variables ();

 private:
   Board* board;
   string file_name;
   ifstream input;
   vector<string> variables;
   vector<string>* values;

   bool load_variables ();
   bool load_values ();
   bool load_file ();

   bool is_empty_line (const string& line) const;
   bool set_variable (string& input);

   bool variable_was_found (const string& input, uint* index_variable);
   bool value_was_found (
       uint index_variable, const string& input, uint *index_value);

   bool set_value (uint index_variable, const string& token, uint index_value);
   bool get_boolean (uint index_value) const;

   void tokenize (const string& input, vector<string>& tokens,
		  const string& delimiters = " ") const;

   Board::GameStatus get_status (uint index_value) const;
   Piece::Player get_turn (uint index_value) const;
   Piece::Player get_color (uint variable) const;
   Board::CastleSide get_side (uint variable) const;
   Piece::Type get_type (uint variable) const;
};

#endif
