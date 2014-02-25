#ifndef NCURSES_H_DEFINED
#define NCURSES_H_DEFINED

#include <stdbool.h>
#include <ncurses.h>
#include <sys/select.h>			// select()
#include <unistd.h>				// STDIN_FILENO

/// Everything related to the terminal library Ncurses.
namespace Ncurses
{
	/// Initializes Ncurses mode.
	bool init();

	/// Quits Ncurses mode.
	void exit();

	/// Sleeps for #delay miliseconds.
	void delay_ms(int delay);

	/// Returns a pressed character within a
	/// timespan of #delay_ms (milliseconds).
	///
	/// @note If you send -1, it'll block the execution,
	///       waiting for the input indefinitely.
	///
	/// @note It's #int because nCurses uses some other
	///       values that don't fit on a #char variable.
	int getInput(int delay_ms=-1);
};

#endif //NCURSES_H_DEFINED

