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
   FileReader  ();              
   FileReader  (const string& filename, Board* board);
   ~FileReader ();
                
   bool set_variables ();

 private:
   Board*          board;
   string          file_name;
   ifstream        input;
   vector<string>  variables;
   vector<string>* values; 
                
   bool load_variables ();            
   bool load_values ();                 
   bool load_file ();
                        
   bool is_empty_line      (const string& line) const;
   bool set_variable       (string& input);
                        
   bool variable_was_found (const string& input, u_int* index_variable);
   bool value_was_found    (u_int index_variable, const string& input, 
                            u_int *index_value);
   bool set_value          (u_int index_variable, const string& token, 
                            u_int index_value);
   bool get_boolean        (u_int index_value) const;

   void tokenize (const string& input, vector<string>& tokens, 
		  const string& delimiters = " ") const;

   Board::GameStatus  get_status (u_int index_value) const;
   Piece::Player      get_turn   (u_int index_value) const;
   Piece::Player      get_color  (u_int variable) const;
   Board::CastleSide  get_side   (u_int variable) const;
   Piece::Type        get_type   (u_int variable) const;
};

#endif
