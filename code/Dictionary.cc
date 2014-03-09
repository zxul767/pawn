#include "Dictionary.h"
#include "MaeBoard.h"

#include <iostream>

using std::cerr;
using std::endl;

map<ushort, uint> Dictionary::possible_size;
size_t               Dictionary::size;

Dictionary:: Dictionary (uint size)
{
   set_possible_size ();

   if (possible_size.find (size) != possible_size.end ())
      hash_size = possible_size[size];
   else
      hash_size = possible_size[64];

   Dictionary::size = hash_size;
}

void
Dictionary:: set_possible_size ()
{
   possible_size[16]  =  1085137;
   possible_size[32]  =  2159363;
   possible_size[64]  =  4142027;
   possible_size[128] =  8888437;
   possible_size[256] = 17311163;
}

bool
Dictionary:: exists (const board_key& board)
{
	return (entry.find (board) != entry.end ());
}

bool
Dictionary:: add_entry (const board_key& board, int score, 
                        flag accuracy, 
								const Move &best, uint depth)
{
   unordered_map<board_key, hash_info, hasher, comparer>::iterator i = 
      entry.find (board);

   // This board is already in the table, so just try to update it
   if (i != entry.end ())
   {
      if (depth >= (*i).second.depth)
      {
         (*i).second.score = score;
         (*i).second.accuracy = accuracy;
         (*i).second.best = best;
         (*i).second.depth = depth;
         return true;
      }
      return false;
   }

   Dictionary::hash_info data;
	data.score    = score;
	data.accuracy = accuracy;
	data.best     = best;
	data.depth    = depth;

	entry.insert (pair<board_key, hash_info>(board, data));

	return true;
}

bool
Dictionary:: get_data (const board_key& board, Dictionary::hash_info& data)
{
	if (exists (board))
	{
		unordered_map<board_key, hash_info, hasher, comparer>::iterator iter = 
         entry.find (board);

		data = (*iter).second;
		return true;
	}
	
	return false;
}

void
Dictionary:: show_all ()
{
	unordered_map<board_key, hash_info, hasher, comparer>::iterator dictionary_iter;

	cerr << "\nThe original elements [Key : Value] are : ";

	for (dictionary_iter = entry.begin(); 
        dictionary_iter != entry.end(); 
        dictionary_iter++)
	{
		cerr << endl << "    score : " << (dictionary_iter->second).score;
		cerr << endl << " accuracy : " << (dictionary_iter->second).accuracy;
		cerr << endl << "best move : " << (dictionary_iter->second).best;
		cerr << endl << "    depth : " << (dictionary_iter->second).depth;
	}

	cerr << endl;
}


uint
Dictionary::get_size () const
{
   return entry.size ();
}

uint
Dictionary::get_capacity () const
{
   return hash_size;
}

void
Dictionary::reset ()
{
   entry.clear ();
}

