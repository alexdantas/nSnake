
#ifndef TIMER_H_DEFINED
#define TIMER_H_DEFINED

#include <string>
#include <sstream>
#include <sys/time.h>
#include "stdint.h" // for suseconds_t

/// A timer measured in milisseconds.
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

private:
	suseconds_t startMark;
	suseconds_t stopMark;
	suseconds_t pausedMark;
	bool        running;
	bool        paused;
};

#endif /* TIMER_H_DEFINED */

