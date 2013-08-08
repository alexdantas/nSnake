#include <sstream>  // allows me to have intToString()
#include <stdlib.h> // srand()
#include <unistd.h> // usleep()
#include "Ncurses.hpp"
#include "Log.hpp"

// Must intialize static members out of the class
int     Ncurses::framerate       = 0;
int     Ncurses::framerate_delay = 0;
int     Ncurses::frame_delta     = 0;
Timer   Ncurses::framerate_timer;
int     Ncurses::width           = 0;
int     Ncurses::height          = 0;
int     Ncurses::currentWidth    = 0;
int     Ncurses::currentHeight   = 0;
bool    Ncurses::hasColors       = false;
WINDOW* Ncurses::screen          = NULL;

bool Ncurses::init(int width, int height, int frameRate)
{
    Ncurses::width  = width;
    Ncurses::height = height;

    Ncurses::screen = initscr(); // Starting ncurses!

	if (has_colors() == TRUE) // && (global.screen_use_colors))
	{
        Ncurses::hasColors = true;

		start_color();

		// This is a big hack to initialize all possible colors
		// in ncurses. The thing is, all colors are between
		// COLOR_BLACK and COLOR_WHITE.
		// Since I've set a large number of enums covering
		// all possibilities, I can do it all in a for loop.
		// Check 'man init_pair' for more details.
		//
		// This was taken straight from <curses.h>:
		//
		// #define COLOR_BLACK	 0
		// #define COLOR_RED	 1
		// #define COLOR_GREEN	 2
		// #define COLOR_YELLOW	 3
		// #define COLOR_BLUE	 4
		// #define COLOR_MAGENTA 5
		// #define COLOR_CYAN	 6
		// #define COLOR_WHITE	 7

		int i, j, k = 1;
		for (i = (COLOR_BLACK); i <= (COLOR_WHITE); i++)
		{
			for (j = (COLOR_BLACK); j <= (COLOR_WHITE); j++)
			{
				init_pair(k, i, j);
				k++;
			}
		}
	}

	// Gets the current width and height of the terminal
	int current_height, current_width;
	getmaxyx(stdscr, current_height, current_width);

	if ((current_width  < Ncurses::width) ||
	    (current_height < Ncurses::height))
	{
		endwin();
        Log::error("Error! Your console screen is smaller than" +
                   Ncurses::intToString(Ncurses::width) +
                   "x" +
                   Ncurses::intToString(Ncurses::height) +
                   "\n" +
                   "Please resize your window and try again\n");

        throw "Ncurses failed.";
	}

    Ncurses::currentWidth = current_width;
    Ncurses::currentHeight = current_height;

	cbreak();    // Character input doesnt require the <enter> key anymore
	noecho();    // Wont print the keys received through input
//	nodelay(stdscr, TRUE); // Wont wait for input
	keypad(stdscr, TRUE);  // Support for extra keys (F1, F2, ... )
//    timeout(0);  // Won't wait for input.
	refresh();   // Refresh the screen (prints whats in the screen buffer)

    return true;
}
void Ncurses::hideCursor(bool willHide)
{
    if (willHide)
        curs_set(0);
    else
        curs_set(1);
}
void Ncurses::exit()
{
	erase();
	refresh();
	endwin();
}
std::string Ncurses::intToString(int num)
{
    std::stringstream ss;
    ss.str("");
    ss << num;

    std::string numStr = ss.str();
    return numStr;
}
// void Ncurses::framerateWait()
// {
//     frame_delta = framerate_timer.delta();

//     if ((frame_delta) < (framerate_delay))
//         Ncurses::delay_ms((framerate_delay) - frame_delta);

//     framerate_timer.restart();

// // Ncurses's framerate manager SUCKS
// //    Ncurses_framerateDelay(&(fpsManager));
// }
// Uint32 Ncurses::getFramerateDelay()
// {
//     return framerate_delay;
// }
// int Ncurses::getDelta()
// {
//     return frame_delta;
// }
int Ncurses::randomNumberBetween(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}
// void Ncurses::showCursor()
// {
//     Ncurses_ShowCursor(Ncurses_ENABLE);
// }
// void Ncurses::hideCursor()
// {
//     Ncurses_ShowCursor(Ncurses_DISABLE);
// }
// void Ncurses::toggleCursor()
// {
//     if (Ncurses_ShowCursor(Ncurses_QUERY) == Ncurses_ENABLE)
//         Ncurses::hideCursor();
//     else
//         Ncurses::showCursor();
// }
void Ncurses::print(std::string what, int x, int y)
{
    mvaddstr(y, x, what.c_str());
}
void Ncurses::refresh()
{
    wrefresh(Ncurses::screen);
}
void Ncurses::clearScreen()
{
    erase();
}
void Ncurses::delay_us(suseconds_t delay)
{
    usleep(delay);
}
void Ncurses::delay_ms(int delay)
{
    Ncurses::delay_us(delay * 1000);
}
void Ncurses::inputDelay(int delay)
{
    timeout(delay);
}


