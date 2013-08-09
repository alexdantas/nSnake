#include "TimerCounter.hpp"

TimerCounter::TimerCounter(int delay)
{
    this->setDelay(delay);
}
void TimerCounter::setDelay(int delay)
{
    this->delay = delay;
    this->done  = false;
}
void TimerCounter::startCounting()
{
    this->restart();
}
bool TimerCounter::isDone()
{
    if (this->done)             // Previously finished
        return true;

    if (!(this->running))       // Never started
        return false;

    this->pause();

    if (this->delta() > this->delay)
    {
        this->stop();
        return true;
    }

    this->unpause();
    return false;
}

