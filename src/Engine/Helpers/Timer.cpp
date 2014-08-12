#include <Engine/Helpers/Timer.hpp>

#include <iostream>				// NULL

// How many microseconds exists in a second
#define MICRO_IN_SECONDS 1000000

// Local functino that returns current microsecond
// amount since the Epoch.
//
static suseconds_t get_ticks()
{
	struct timeval tmp;
	gettimeofday(&(tmp), NULL);

	return tmp.tv_usec + (tmp.tv_sec * MICRO_IN_SECONDS);
}

Timer::Timer():
	startMark(0),
	stopMark(0),
	pausedMark(0),
	running(false),
	paused(false)
{}
void Timer::start()
{
    this->startMark  = get_ticks();
    this->stopMark   = 0;
    this->pausedMark = 0;
    this->running    = true;
    this->paused     = false;
}
void Timer::pause()
{
    if (!running || paused) return;

    this->pausedMark = get_ticks() - (this->startMark);

    this->running = false;
    this->paused  = true;
}
void Timer::unpause()
{
    if (!paused || running) return;

    this->startMark  = (get_ticks()) - (this->pausedMark);
    this->pausedMark = 0;

    this->running = true;
    this->paused  = false;
}
bool Timer::isRunning()
{
    return this->running;
}
bool Timer::isPaused()
{
    return this->paused;
}
suseconds_t Timer::delta_us()
{
    if (this->isRunning())
	    return get_ticks() - this->startMark;

    if (this->paused)
        return this->pausedMark;

    // Something went wrong here
    if (this->startMark == 0)
        return 0;

    return (this->pausedMark) - (this->startMark);
}
suseconds_t Timer::delta_ms()
{
    return this->delta_us() / 1000;
}
suseconds_t Timer::delta_s()
{
    return this->delta_ms() / 1000;
}

