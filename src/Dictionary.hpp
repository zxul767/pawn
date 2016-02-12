#ifndef DICTIONARY_H
#define DICTIONARY_H

/*==============================================================================
  Implements a transposition table, used to improve performance of search
  algorithms such as iterative deepening search
  ==============================================================================*/

#include <unordered_map>
#include <map>

#include "Util.hpp"
#include "Move.hpp"
#include "MaeBoard.hpp"

namespace game_engine
{
class Dictionary
{
  public:

   enum flag { EXACT, UPPER_BOUND, LOWER_BOUND, UNKNOWN };

   struct board_key
   {
      ullong hash_key;
      ullong hash_lock;
   };

   struct hash_info
   {
      int score;
      flag accuracy;
      game_rules::Move best_move;
      ushort depth;
   };

   Dictionary (uint hash_size = 0);

   bool add_entry (
       const board_key& board, int score, flag accuracy,
       const game_rules::Move& best_move, uint depth);

   bool exists (const board_key& board);
   bool get_data (const board_key& board, hash_info& data);
   void show_all ();
   uint get_size () const;
   uint get_capacity () const;
   void reset ();

   static void set_possible_size ();

   static std::map<ushort, uint> possible_size;
   static const ushort MAX_MEMORY = 256;
   static size_t size;

  private:
   class hasher
   {
     public:
      size_t operator ()(const board_key& board) const
      {
         return (size_t) board.hash_key % size;
      }
   };

   class comparer
   {
     public:
      bool operator ()(const board_key& board, const board_key& other) const
      {
         return board.hash_lock == other.hash_lock;
      }
   };

   uint hash_size;
   std::unordered_map<board_key, hash_info, hasher, comparer> entry;
};

} // namespace game_engine

#endif // HASH_DICTIONARY_H
