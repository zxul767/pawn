#include <sstream>
#include <iostream>
#include <cstdlib>

#include "Timer.h"

Timer::Timer (double time_out)
{
   this->time_out = time_out;
   this->time_elapsed = -1;
   this->begin = clock ();
}

Timer::~Timer ()
{
}

void
Timer::start ()
{
   this->time_elapsed = -1;
   this->begin = clock ();
}

void
Timer::stop ()
{
   this->end = clock ();
}

void
Timer::set_clock (double time_out)
{
   this->time_out = time_out;
   this->time_elapsed = -1;
   this->begin = clock ();
}

bool
Timer::is_time_out ()
{
   this->end = clock ();
   this->time_elapsed = (double) (this->end - this->begin) / CLOCKS_PER_SEC;

   return fabs(this->time_elapsed - this->time_out) <= EPSILON;
}

double
Timer::elapsed_time ()
{
   if (this->time_elapsed < 0)
   {
      this->end = clock ();
      this->time_elapsed = (double) (this->end - this->begin) / CLOCKS_PER_SEC;
   }
   return this->time_elapsed;
}

void
Timer::sleep () const
{
   if (this->time_out > 0)
   {
      std::ostringstream sleep_command;
      sleep_command << "sleep " << this->time_out;
      if (system (sleep_command.str().c_str ()))
      {
         std::cerr << "sleep command failed" << std::endl;
      }
   }
}
