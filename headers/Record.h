#ifndef RECORD_H
#define RECORD_H

#include <ext/hash_map>
#include "common.h"

using namespace __gnu_cxx;

class Record
{
 public:
   struct board_key
   {
      u_llong hash_key;
      u_llong hash_lock;
   };
   
   Record () {}

   bool    add_record      (const board_key& key, u_short&);
   bool    decrease_record (const board_key& key);
   u_short get_repetitions (const board_key& key) const;
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
  
   hash_map <board_key, u_short, hasher, hasher> record;
};

#endif // RECORD_H
