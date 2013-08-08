// imago - Tetris(tm) on the console.
// Copyright (C) 2013 Alexandre Dantas (kure)
//
// This file is part of imago.
//
// imago is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// homepage: http://www.github.com/alexdantas/imago/
// mailto:   alex.dantas92@gmail.com
////////////////////////////////////////////////////////////////////////

#include <unistd.h> // usleep()
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
void Timer::delay_us(suseconds_t delay)
{
    usleep(delay);
}
void Timer::delay_ms(int delay)
{
    Timer::delay_us(delay * 1000);
}

