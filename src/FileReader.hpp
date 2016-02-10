#ifndef FILE_READER_H
#define FILE_READER_H

/*==============================================================================
  Reads and writes chess games in an ad-hoc notation (see initial.in)
 ==============================================================================*/

#include <fstream>
#include <string>
#include <vector>

#include "Board.hpp"

class FileReader
{
 public:
   FileReader ();
   FileReader (const std::string& filename, Board* board);
   ~FileReader ();

   bool set_variables ();

 private:
   Board* board;
   std::string file_name;
   std::ifstream input;
   std::vector<std::string> variables;
   std::vector<std::string>* values;

   bool load_variables ();
   bool load_values ();
   bool load_file ();

   bool is_empty_line (const std::string& line) const;
   bool set_variable (std::string& input);

   bool variable_was_found (const std::string& input, uint* index_variable);
   bool value_was_found (
       uint index_variable, const std::string& input, uint *index_value);

   bool set_value (uint index_variable, const std::string& token, uint index_value);
   bool get_boolean (uint index_value) const;

   void tokenize (const std::string& input, std::vector<std::string>& tokens,
		  const std::string& delimiters = " ") const;

   Board::GameStatus get_status (uint index_value) const;
   Piece::Player get_turn (uint index_value) const;
   Piece::Player get_color (uint variable) const;
   Board::CastleSide get_side (uint variable) const;
   Piece::Type get_type (uint variable) const;
};

#endif
