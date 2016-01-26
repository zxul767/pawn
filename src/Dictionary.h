#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <unordered_map>
#include <map>

#include "common.h"
#include "Move.h"

using std::map;

#include "MaeBoard.h"

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
      Move best_move;

      ushort depth;
   };

   Dictionary (uint hash_size = 0);

   bool add_entry (
       const board_key& board, int score, flag accuracy, const Move& best_move,
       uint depth);
   bool exists (const board_key& board);
   bool get_data (const board_key& board, hash_info& data);
   void show_all ();
   uint get_size () const;
   uint get_capacity () const;
   void reset ();

   static void set_possible_size ();

   static map<ushort, uint> possible_size;
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
   unordered_map<board_key, hash_info, hasher, comparer> entry;
};

#endif // HASH_DICTIONARY_H
