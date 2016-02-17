#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

/*==============================================================================
  Implements a transposition table, used to improve performance of search
  algorithms such as iterative deepening search
  ==============================================================================*/

#include <unordered_map>
#include <map>

#include "Util.hpp"
#include "Move.hpp"
#include "MaeBoard.hpp"
#include "BoardKey.hpp"

namespace game_engine
{
using game_rules::BoardKey;

class TranspositionTable
{
  public:
   enum flag { EXACT, UPPER_BOUND, LOWER_BOUND, UNKNOWN };

   struct BoardEntry
   {
      int score;
      flag accuracy;
      game_rules::Move best_move;
      ushort depth;
   };

   TranspositionTable (uint hash_size = 0);

   bool add_entry (
       const BoardKey&, int score, flag accuracy,
       const game_rules::Move& best_move, uint depth);

   bool exists (const BoardKey&);
   bool get_entry (const BoardKey&, BoardEntry&);
   uint get_size () const;
   uint get_capacity () const;
   void reset ();

   static void set_possible_size ();

   static std::map<ushort, uint> possible_size;
   static const ushort MAX_MEMORY = 256;
   static size_t size;

  private:
   class board_hasher
   {
     public:
      size_t operator ()(const BoardKey& key) const
      {
         return (size_t) key.hash_key % size;
      }
   };

   class board_comparer
   {
     public:
      bool operator ()(const BoardKey& key, const BoardKey& other_key) const
      {
         return key.hash_lock == other_key.hash_lock;
      }
   };

   uint hash_size;
   std::unordered_map<BoardKey, BoardEntry, board_hasher, board_comparer> entries;
};

} // namespace game_engine

#endif // HASH_TRANSPOSITION_TABLE_H
