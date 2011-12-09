#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <ext/hash_map>
#include <map>

#include "common.h"
#include "Move.h"

using namespace __gnu_cxx;
using std::map;

#include "MaeBoard.h"

class Dictionary
{

	public:
   
      enum flag {EXACT, UBOUND, LBOUND, UNKNOWN};
   
	   struct board_key
      {
         u_llong hash_key;
         u_llong hash_lock;
      };

		struct hash_info
		{
			int     score;
			flag    accuracy;
			Move    best;

			u_short depth;
		};

		Dictionary (u_int hash_size = 0);

		bool  add_entry (const board_key& board, int score, 
                       flag accuracy, 
                       const Move& best, u_int depth);
		bool  exists    (const board_key& board);
		bool  get_data  (const board_key& board, hash_info& data);
		void  show_all  ();
      u_int get_size  () const;
      u_int get_capacity () const;
      void  reset () ;

      static void set_possible_size ();

      static map<u_short, u_int> possible_size;
      static const u_short       MAX_MEMORY = 256;
      static size_t              size;

	private:		
      struct hasher
      {
         size_t operator ()(const board_key& board) const
         {
            return (size_t) board.hash_key % size;
         }

         bool operator ()(const board_key& board, const board_key& other) const
         {
            return board.hash_lock == other.hash_lock;
         }
      };

      u_int     hash_size;
      hash_map  <board_key, hash_info, hasher, hasher> entry;	  
};

#endif // HASH_DICTIONARY_H

