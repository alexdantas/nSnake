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

	int getInput(int delay_ms);
};

#endif //NCURSES_H_DEFINED

