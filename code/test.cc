#include "common.h"
#include <iostream>

using namespace std;

int
main ()
{
   for (uint i = 0; i < 100; ++i)
      cout << Util::random (0.0, 1.0) << endl;

   return 0;
}
