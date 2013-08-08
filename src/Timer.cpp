#include "Timer.hpp"

#define MICROSSECONDS_IN_SECONDS 1000000

/// Local function that returns the ticks (number of microsseconds)
//  since the Epoch.
static suseconds_t getTicks()
{
    struct timeval tmp;
    gettimeofday (&(tmp), NULL);

    return tmp.tv_usec + (tmp.tv_sec * MICROSSECONDS_IN_SECONDS);
}

Timer::Timer()
{
    this->startMark  = 0;
    this->stopMark   = 0;
    this->pausedMark = 0;
    this->running    = false;
    this->paused     = false;
}
void Timer::start()
{
    if (this->running) return;

    this->startMark  = getTicks();
    this->stopMark   = 0;
    this->pausedMark = 0;
    this->running    = true;
    this->paused     = false;
}
void Timer::stop()
{
    if (!this->running) return;

    this->stopMark = getTicks();
    this->running  = false;
    this->paused   = false;
}
void Timer::restart()
{
    this->stop();
    this->start();
}
void Timer::pause()
{
    if (!running || paused) return;

    this->running    = false;
    this->paused     = true;
    this->pausedMark = (getTicks()) - (this->startMark);
}
void Timer::unpause()
{
    if (!paused || running) return;

    this->running = true;
    this->paused  = false;

    this->startMark  = (getTicks()) - (this->pausedMark);
    this->pausedMark = 0;
}
bool Timer::isRunning()
{
    return (this->running);
}
bool Timer::isPaused()
{
    return (this->paused);
}
suseconds_t Timer::delta()
{
    if (this->isRunning())
        return (this->currentTime());

    if (this->paused)
        return (this->pausedMark);

    // Something went wrong here
    if (this->startMark == 0)
        return 0;

    return ((this->stopMark) - (this->startMark));
}
suseconds_t Timer::delta_ms()
{
    return (this->delta() % 1000);
}
suseconds_t Timer::delta_s()
{
    return (this->delta_ms() / 1000);
}
suseconds_t Timer::delta_m()
{
    return (this->delta_s() / 60);
}
suseconds_t Timer::currentTime()
{
    return (getTicks()) - (this->startMark);
}

