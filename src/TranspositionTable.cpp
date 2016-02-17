#include "TranspositionTable.hpp"
#include "MaeBoard.hpp"

#include <iostream>

namespace game_engine
{
using game_rules::Move;
using std::unordered_map;

std::map<ushort, uint> TranspositionTable::possible_size;
size_t TranspositionTable::size;

TranspositionTable::TranspositionTable (uint size)
{
   set_possible_size ();

   if (TranspositionTable::possible_size.find (size) != TranspositionTable::possible_size.end ())
      this->hash_size = TranspositionTable::possible_size[size];
   else
      this->hash_size = TranspositionTable::possible_size[64];

   TranspositionTable::size = this->hash_size;
}

void
TranspositionTable::set_possible_size ()
{
   TranspositionTable::possible_size[16] = 1085137;
   TranspositionTable::possible_size[32] = 2159363;
   TranspositionTable::possible_size[64] = 4142027;
   TranspositionTable::possible_size[128] = 8888437;
   TranspositionTable::possible_size[256] = 17311163;
}

bool
TranspositionTable::exists (const BoardKey& board)
{
   return this->entries.find (board) != this->entries.end ();
}

bool
TranspositionTable::add_entry (
    const BoardKey& key, int score, flag accuracy, const Move& best_move, uint depth)
{
   auto iter = this->entries.find (key);

   // This board is already in the table, so just try to update it
   if (iter != this->entries.end ())
   {
      if (depth >= (*iter).second.depth)
      {
         (*iter).second.score = score;
         (*iter).second.accuracy = accuracy;
         (*iter).second.best_move = best_move;
         (*iter).second.depth = depth;
         return true;
      }
      return false;
   }

   TranspositionTable::BoardEntry entry;
   entry.score = score;
   entry.accuracy = accuracy;
   entry.best_move = best_move;
   entry.depth = depth;

   this->entries.insert (std::pair<BoardKey, BoardEntry>(key, entry));

   return true;
}

bool
TranspositionTable::get_entry (const BoardKey& key, TranspositionTable::BoardEntry& entry)
{
   if (exists (key))
   {
      auto iter = this->entries.find (key);
      entry = (*iter).second;
      return true;
   }

   return false;
}

uint
TranspositionTable::get_size () const
{
   return this->entries.size ();
}

uint
TranspositionTable::get_capacity () const
{
   return this->hash_size;
}

void
TranspositionTable::reset ()
{
   this->entries.clear ();
}

} // namespace game_engine
