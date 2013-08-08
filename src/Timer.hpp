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

/// @file Timer.hpp Counts time through execution.
//
//	This class implements a timer that counts in milisseconds.
//
//	How-to-use:
//
//	Timer timer;
//	timer.start();
//	...
//	timer.pause();
//	...
//	timer.unpause();
//	...
//	timer.stop();
//	int delta = timer.delta();
//
//	For example:
//

#ifndef TIMER_H_DEFINED
#define TIMER_H_DEFINED

#include <string>
#include <sstream>
#include <sys/time.h>
#include "stdint.h" // for suseconds_t

/// A timer measured in milisseconds.
class Timer
{
public:
	Timer();

	/// Sets a starting point for the timer.
	void start();

	/// Sets a stopping point for the timer.
	void stop();

	/// Temporarily stops the timer.
	void pause();

	/// Stops and starts the timer.
	void restart();

	/// Restarts the timer if it was paused.
	void unpause();

	/// Tells if the timer's still running (hasn't called stop())
	bool isRunning();

	/// Tells if the timer's paused.
	bool isPaused();

	/// Returns the whole timer's difference in microseconds.
	//	@note If the timer's not started, will return 0.
	suseconds_t delta();

	/// Returns the milisseconds part of the timer's difference.
	suseconds_t delta_ms();

	/// Returns the seconds part of the timer's difference.
	suseconds_t delta_s();

    /// Returns the minutes part of the timer's differente.
    suseconds_t delta_m();

	/// Returns the difference between timer's start point and now.
	suseconds_t currentTime();

    /// Pauses the program's execution for *delay* microseconds.
    ///
    /// Can be called independently of any class instance.
    static void delay_us(suseconds_t delay);

    static void delay_ms(int delay);

private:
	suseconds_t startMark;
	suseconds_t stopMark;
	suseconds_t pausedMark;
	bool        running;
	bool        paused;
};

#endif /* TIMER_H_DEFINED */

