#include <Interface/Layout.hpp>
#include <Interface/Ncurses.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

#include <iostream>
#include <cstdlib>

int Layout::screenWidth  = 0;
int Layout::screenHeight = 0;

static int intendedWidth;
static int intendedHeight;

Layout::Layout(int width, int height):
	main(NULL)
{
	intendedWidth  = width;
	intendedHeight = height;
}
Layout::~Layout()
{ }
void Layout::windowsInit()
{
	clear();

	// Gets the current width and height of the whole terminal.
	int current_height, current_width;
	getmaxyx(stdscr, current_height, current_width);

	if ((current_width  < intendedWidth) ||
	    (current_height < intendedHeight))
	{
		Ncurses::exit();
		std::cerr << "Error! Your console screen is smaller than"
		          << intendedWidth << "x" << intendedHeight << "\n"
		          << "Please resize your window and try again"
		          << std::endl;

		exit(EXIT_FAILURE);
	}

	// Sets global info
	Layout::screenWidth  = current_width;
	Layout::screenHeight = current_height;

	// Creating the main window for this layout.
	// We'll center based on user's settings
	int main_x = 0;
	int main_y = 0;

	if (Globals::Screen::center_horizontally)
		main_x = current_width/2 - intendedWidth/2;

	if (Globals::Screen::center_vertically)
		main_y = current_height/2 - intendedHeight/2;

	this->main = new Window(main_x,
	                        main_y,
	                        intendedWidth,
	                        intendedHeight);

	if ((Globals::Screen::outer_border) &&
	    (Globals::Screen::show_borders))
	{
		this->main->borders(Globals::Screen::fancy_borders ?
		                    Window::BORDER_FANCY :
		                    Window::BORDER_REGULAR);
	}

	this->main->refresh();
}
void Layout::windowsExit()
{
	SAFE_DELETE(this->main);
}
void Layout::draw()
{
	// When subclassing, make sure to implement this!
}

