#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <cmath>
#include "common.h"

class Timer
{
 public:
   Timer (double time_out = 1.0);
   ~Timer ();

   void   set_clock (double time_out);
   bool   is_time_out  ();
   double elapsed_time ();
   void   start ();
   void   stop  ();
   void   sleep () const;

 private:
   double  time_out;
   double  time_elapsed;
   clock_t begin, end;

   static const double EPSILON = 1E-7;
};

#endif // TIMER_H
