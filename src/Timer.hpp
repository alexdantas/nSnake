#ifndef TIMER_H_DEFINED
#define TIMER_H_DEFINED

#include <string>
#include <sstream>

/// General-purpose timer with milisecond precision.
///
///	## How to use:
///
///	    Timer timer;
///	    timer.start();
///	    // ...
///	    timer.pause();
///	    // ...
///	    timer.unpause();
///	    // ...
///	    timer.stop();
///	    int delta = timer.delta();
///
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

	/// Returns how many _miliseconds_ have passed.
	///
	///	@note If the timer's not started, will return 0.
	int delta();

	/// Returns the milisseconds part of the timer's difference.
	///
	/// Don't confuse this method with delta().
	///
	/// @note This is the same as *(delta() % 1000)*.
	int delta_ms();

	/// Returns how many _seconds_ have passed.
	int delta_s();

    /// Returns how many _seconds_ have passed in _float_.
	float delta_sf();

protected:
	unsigned int startMark;
	unsigned int stopMark;
	unsigned int pausedMark;

	bool running;
	bool paused;
};

#endif /* TIMER_H_DEFINED */

