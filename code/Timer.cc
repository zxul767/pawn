#include <sstream>
#include <iostream>
#include "Timer.h"

Timer::Timer (double time_out)
{
   this->time_out = time_out;
   time_elapsed = -1;
   begin = clock ();
}

Timer::~Timer ()
{
}

void
Timer::start ()
{
   time_elapsed = -1;
   begin = clock ();
}

void
Timer::stop ()
{
   end = clock ();
}

void 
Timer::set_clock (double time_out)
{
   this->time_out = time_out;
   time_elapsed = -1;
   begin = clock ();
}

bool
Timer::is_time_out ()
{
   end = clock ();
   time_elapsed = (double) (end-begin) / CLOCKS_PER_SEC;

   return (fabs(time_elapsed - time_out) <= EPSILON);
}

double
Timer::elapsed_time ()
{
   if (time_elapsed < 0)
   {
      end = clock ();
      time_elapsed = (double) (end-begin) / CLOCKS_PER_SEC;
   }
   return time_elapsed;
}

void
Timer::sleep () const
{   
   if (time_out > 0)
   {
      std::ostringstream sleep_command;
      sleep_command << "sleep " << time_out;
      system (sleep_command.str().c_str ());
   }
}
