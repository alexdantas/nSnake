#ifndef TIMERCOUNTER_H_DEFINED
#define TIMERCOUNTER_H_DEFINED

#include "Timer.hpp"

/// Counter that tells us if a certain time has elapsed, with
/// milisecond precision.
///
/// ## How to use:
///
///     TimerCounter timer(1000); // 1 second
///     timer.startCounting();
///     // ...
///     if (timer.isDone())
///         // do something
///
class TimerCounter: public Timer
{
public:
    /// Creates a counter timer with *delay*.
	TimerCounter(int delay);

    /// Changes the timer's delay to *delay*.
    void setDelay(int delay);

    /// Makes the timer start counting.
    ///
    /// @note Don't worry, you can call this as many times
    ///       you want, it'll restart itself.
    void startCounting();

    /// Tells us if the previously set delay has passed.
    bool isDone();

private:
    /// The delay we must wait for.
    int delay;

    /// If we've reached the delay we was set for.
    bool done;
};

#endif /* TIMERCOUNTER_H_DEFINED */

