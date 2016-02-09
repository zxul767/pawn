#include "Dictionary.h"
#include "MaeBoard.h"

#include <iostream>

using std::unordered_map;

std::map<ushort, uint> Dictionary::possible_size;
size_t Dictionary::size;

Dictionary::Dictionary (uint size)
{
   set_possible_size ();

   if (Dictionary::possible_size.find (size) != Dictionary::possible_size.end ())
      this->hash_size = Dictionary::possible_size[size];
   else
      this->hash_size = Dictionary::possible_size[64];

   Dictionary::size = this->hash_size;
}

void
Dictionary::set_possible_size ()
{
   Dictionary::possible_size[16] = 1085137;
   Dictionary::possible_size[32] = 2159363;
   Dictionary::possible_size[64] = 4142027;
   Dictionary::possible_size[128] = 8888437;
   Dictionary::possible_size[256] = 17311163;
}

bool
Dictionary::exists (const board_key& board)
{
   return this->entry.find (board) != this->entry.end ();
}

bool
Dictionary::add_entry (
    const board_key& board, int score, flag accuracy, const Move& best_move, uint depth)
{
   unordered_map<board_key, hash_info, hasher, comparer>::iterator i =
         this->entry.find (board);

   // This board is already in the table, so just try to update it
   if (i != this->entry.end ())
   {
      if (depth >= (*i).second.depth)
      {
         (*i).second.score = score;
         (*i).second.accuracy = accuracy;
         (*i).second.best_move = best_move;
         (*i).second.depth = depth;
         return true;
      }
      return false;
   }

   Dictionary::hash_info data;
   data.score = score;
   data.accuracy = accuracy;
   data.best_move = best_move;
   data.depth = depth;

   this->entry.insert (std::pair<board_key, hash_info>(board, data));

   return true;
}

bool
Dictionary::get_data (const board_key& board, Dictionary::hash_info& data)
{
   if (exists (board))
   {
      unordered_map<board_key, hash_info, hasher, comparer>::iterator iter =
            this->entry.find (board);

      data = (*iter).second;
      return true;
   }

   return false;
}

void
Dictionary::show_all ()
{
   unordered_map<board_key, hash_info, hasher, comparer>::iterator dictionary_iter;

   std::cerr << "\nThe original elements [Key : Value] are : ";

   for (dictionary_iter = this->entry.begin();
        dictionary_iter != this->entry.end();
        dictionary_iter++)
   {
      std::cerr << std::endl << "    score : " << (dictionary_iter->second).score;
      std::cerr << std::endl << " accuracy : " << (dictionary_iter->second).accuracy;
      std::cerr << std::endl << "best move : " << (dictionary_iter->second).best_move;
      std::cerr << std::endl << "    depth : " << (dictionary_iter->second).depth;
   }

   std::cerr << std::endl;
}


uint
Dictionary::get_size () const
{
   return this->entry.size ();
}

uint
Dictionary::get_capacity () const
{
   return this->hash_size;
}

void
Dictionary::reset ()
{
   this->entry.clear ();
}
