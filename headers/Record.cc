#include "Record.h"

bool
Record:: add_record (const board_key key, u_short& times)
{
   hash_map<board_key, u_short, hasher, hasher>::iterator i = 
      record.find (key);

   if (i != record.end ())
   {
      (*i).second ++;
      times = (*i).second;
      
      return false;
   }

   record.insert (pair<board_key, u_short>(key, 1));

   return true;
}

bool
Record:: decrese_record (const board_key key)
{
   hash_map<board_key, u_short, hasher, hasher>::iterator i = 
      record.find (key);

   if (i != record.end())
   {
      if ((*i).second <= 1)
         record.erase (i);
      else
         (*i).second --;

      return true;
   }
   
   return false;
}

void 
Record:: reset ()
{
   record.clear ();
}
