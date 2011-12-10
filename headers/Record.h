#ifndef RECORD_H
#define RECORD_H

#include <unordered_map>
#include "common.h"

using namespace std;

class Record
{
public:
   struct board_key
   {
      ullong hash_key;
      ullong hash_lock;
   };

   Record () {}

   bool    add_record      (const board_key& key, ushort&);
   bool    decrease_record (const board_key& key);
   ushort get_repetitions (const board_key& key) const;
   void    reset ();

   static const size_t size = 4142027;

private:
   struct hasher
   {
      size_t operator ()(const board_key& board) const
         {
            return (size_t) (board.hash_key % size);
         }

      bool operator ()(const board_key& board, const board_key& other) const
         {
            return board.hash_lock == other.hash_lock;
         }
   };

   unordered_map <board_key, ushort, hasher, hasher> record;
};

#endif // RECORD_H
