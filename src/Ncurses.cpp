#include <sstream>  // allows me to have intToString()
#include <stdlib.h> // srand()
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
    if (Ncurses::screen == NULL)
    {
        Log::error("Error! Failed initializing ncurses!");
        throw "Ncurses failed to initialize.";
	}

    // Starts color support if the terminal allows it
    Color::init();

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

        throw "Ncurses failed to initialize.";
	}

    Ncurses::currentWidth  = current_width;
    Ncurses::currentHeight = current_height;

	cbreak();    // Character input doesnt require the <enter> key anymore
	noecho();    // Wont print the keys received through input
//	nodelay(stdscr, TRUE); // Wont wait for input
	keypad(stdscr, TRUE);  // Support for extra keys (F1, F2, ... )
//    timeout(0);  // Won't wait for input.
	refresh();   // Refresh the screen (prints whats in the screen buffer)

    // Now we handle the framerate.
    // If we don't do this correctly, the game will always
    // spend 100% CPU usage doing.. nothing
    framerate       = frameRate;
    framerate_delay = (1000/framerate); // 1 second in miliseconds

    framerate_timer.start();

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
    // little hack to convert from integer to string
    std::stringstream ss;
    ss.str("");
    ss << num;

    std::string numStr = ss.str();
    return numStr;
}
void Ncurses::framerateWait()
{
    frame_delta = framerate_timer.delta_ms();

    if ((frame_delta) < (framerate_delay))
        Ncurses::delay_ms(framerate_delay - frame_delta);

    framerate_timer.restart();
}
int Ncurses::getFramerateDelay()
{
    return framerate_delay;
}
int Ncurses::getDelta()
{
    return frame_delta;
}
float Ncurses::getDeltaSeconds()
{
    // frame_delta is the number of miliseconds between last
    // frame and this one.
    //
    // We'll transform it in floaty SECONDS.

    return ((float)(frame_delta))/1000.0;
}
int Ncurses::randomNumberBetween(int min, int max)
{
    // Just in case the caller didn't read
    // the method's documentation.
    if (min > max)
    {
        // swap
        int tmp = max;
        max = min;
        min = tmp;
    }

    return (rand() % (max - min + 1) + min);
}
void Ncurses::print(std::string what, int x, int y)
{
    mvaddstr(y, x, what.c_str());
}
void Ncurses::setStyle(unsigned long pair)
{
    attrset(pair);
}
void Ncurses::refresh()
{
    wrefresh(Ncurses::screen);
}
void Ncurses::clearScreen()
{
    erase();
}
void Ncurses::delay_us(useconds_t delay)
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

