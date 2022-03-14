#ifndef TIMER_H
#define TIMER_H

#include "Util.hpp"
#include <cmath>
#include <ctime>

namespace diagnostics
{
class Timer
{
  public:
    Timer(double time_out = 1.0);
    ~Timer();

    void set_timer(double time_out);
    bool has_timed_out();
    double elapsed_time();
    void start();
    void stop();
    void sleep() const;

  private:
    double time_out;
    double time_elapsed;
    clock_t begin, end;

    static constexpr double EPSILON = 1E-7;
};

} // namespace diagnostics

#endif // TIMER_H
