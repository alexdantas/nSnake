#include <Engine/Graphics/Ncurses.hpp>

#include <ncurses.h>
#include <sys/select.h>			// select()
#include <unistd.h>				// STDIN_FILENO

bool Ncurses::init()
{
	initscr();
	// TODO check for failing

	cbreak();    // Character input doesnt require the <enter> key anymore
	curs_set(0); // Makes the blinking cursor invisible
	noecho();    // Wont print the keys received through input
	nodelay(stdscr, TRUE); // Wont wait for input
	keypad(stdscr, TRUE);  // Support for extra keys (life F1, F2, ... )

	// Ncurses' global variable meaning number of milliseconds
	// to wait after the user presses ESC.
	//
	// VIM uses 25ms, so should you.
	// Source: http://en.chys.info/2009/09/esdelay-ncurses/
	ESCDELAY = 25;

	refresh();   // Refresh the layout (prints whats in the layout bu
	return true;
}

void Ncurses::exit()
{
	erase();
	refresh();
	endwin();
}

int Ncurses::getInput(int delay_ms)
{
	// Will use the great (but complicated) select() function.
	// Check out its manpage for more info.
	int retval = 0;
	int c      = 0;

	fd_set input;
	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = delay_ms * 1000; // microseconds

	// If #delay_ms is -1, we'll wait infinitely
	// (sending NULL to #select())
	struct timeval* timeout_p = NULL;
	if (delay_ms != -1)
		timeout_p = &timeout;

	FD_ZERO(&input);
	FD_SET(STDIN_FILENO, &input);

	// This function is somewhat complex
	// check 'man select' for info
	retval = select(FD_SETSIZE, &input, NULL, NULL, timeout_p);

	// Ncurses' function that works without delay
	// (because we nodelay()'ed)
	c = getch();

	if ((retval == 1) && (c == ERR)) // ERROR
		return -1;

	if (retval == 0)
		return ERR; // Nothing was pressed.
					// This is an Ncurses internal value.

	return c;
}

