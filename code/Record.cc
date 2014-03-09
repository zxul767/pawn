#include "Record.h"

bool
Record:: add_record (const board_key& key, ushort& times)
{
   unordered_map<board_key, ushort, hasher, comparer>::iterator i = 
      record.find (key);

   if (i != record.end ())
   {
      (*i).second ++;
      times = (*i).second;
      
      return false;
   }

   record.insert (pair<board_key, ushort>(key, 1));

   return true;
}

bool
Record:: decrease_record (const board_key& key)
{
   unordered_map<board_key, ushort, hasher, comparer>::iterator i = 
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

ushort
Record::get_repetitions (const board_key& key) const
{
   unordered_map<board_key, ushort, hasher, comparer>::const_iterator i = 
      record.find (key);

   if (i != record.end ())
      return (*i).second;

   return 0;
}

void 
Record:: reset ()
{
   record.clear ();
}
