#include "Timer.h"
#include <iostream>
using namespace std;

int
main ()
{
   Timer time (1.5);

   cout << "Starting in 1.5 seconds..." << endl;
   time.sleep ();
   cout << "Igniting all systems!" << endl;

   return 0;
}
