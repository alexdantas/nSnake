#include <Interface/Window.hpp>

#include <sstream>				// stringstream
#include <iostream>

/* dag-nabbit, PDCurses (windows) doesnt have 'mvwhline' */
#if OS_IS_WINDOWS
#define mvwhline my_mvwhline
#endif

/**
 * PDCurses (on Windows) doesn't have this function, so I need
 * to re-implement it.
 *
 * @todo implement more features - see 'man mvwhline'
 */
void my_mvwhline(WINDOW* win, int y, int x, chtype ch, int num)
{
	int i;
	for (i = 0; i < num; i++)
		mvwaddch(win, y, (x + i), ch);
}

Window::Window(int x, int y, int w, int h):
	win(NULL),
	error(false),
	x(x),
	y(y),
	width(w),
	height(h),
	borderType(BORDER_NONE),
	topLeftTitle(""),
	topRightTitle(""),
	bottomLeftTitle(""),
	bottomRightTitle("")
{
	this->win = newwin(height, width, y, x);

	if (!this->win)
		this->error = true;
}
Window::Window(Window* parent, int x, int y, int width, int height):
	win(NULL),
	error(false),
	borderType(BORDER_NONE),
	topLeftTitle(""),
	topRightTitle(""),
	bottomLeftTitle(""),
	bottomRightTitle("")
{
	// By sending any parameter as 0, we want it to expand
	// until possible.
	// Let's expand based if the parent window has borders
	// or not.
	if (parent->borderType == BORDER_NONE)
	{
		if (width  == 0) width  = parent->width;
		if (height == 0) height = parent->height;
	}
	else
	{
		// Has borders
		if (x == 0) x = 1;
		if (y == 0) y = 1;

		if (width  == 0) width  = parent->width  - 2;
		if (height == 0) height = parent->height - 2;
	}

	this->x = x;
	this->y = y;
	this->width  = width;
	this->height = height;

	this->win = derwin(parent->win, height, width, y, x);
	if (!win)
		this->error = true;
}
Window::~Window()
{
	if (this->win)
		delwin(this->win);
}
bool Window::isValid()
{
	return !(this->error);
}
void Window::resize(int w, int h)
{
	wresize(this->win, h, w);
	this->width  = w;
	this->height = h;
}
void Window::print(std::string str, int x, int y, ColorPair pair)
{
	Colors::pairActivate(this->win, pair);

	mvwaddstr(this->win, y, x, str.c_str());
}
void Window::print(std::vector<std::string> lines, int x, int y, ColorPair pair)
{
	for (size_t i = 0; i < lines.size(); i++)
		this->print(lines[i], x, y + i, pair);
}
void Window::printChar(int c, int x, int y, ColorPair pair)
{
	Colors::pairActivate(this->win, pair);

	mvwaddch(this->win, y, x, c);
}
void Window::setBackground(chtype ch, ColorPair pair)
{
	wbkgd(this->win, ch | pair);
}
void Window::refresh()
{
	//wrefresh(this->win);

	// I've changed all calls to wrefresh() to wnoutrefresh
	// because when I have several WINDOW*, it gets heavy
	// to do the former.
	//
	// As a tradeoff, I need to call `refresh()` at the end
	// of every draw cycle.
	//
	wnoutrefresh(this->win);
}
void Window::clear()
{
	werase(this->win);

	// Redrawing borders if existing
	if (this->borderType != BORDER_NONE)
		this->borders(this->borderType);

	// Now, to the titles!
	if (! this->topLeftTitle.empty())
	{
		this->print(this->topLeftTitle,
		            1, 0,
		            Colors::pair(COLOR_BLUE, COLOR_DEFAULT));
	}
	if (! this->bottomLeftTitle.empty())
	{
		this->print(this->bottomLeftTitle,
		            0, this->getH() - 1,
		            Colors::pair(COLOR_BLUE, COLOR_DEFAULT));
	}
	if (! this->topRightTitle.empty())
	{
		int x = (this->getW() - 1);
		int w = this->topRightTitle.size();

		this->print(this->topRightTitle,
		            x - w, 0,
		            Colors::pair(COLOR_BLUE, COLOR_DEFAULT));
	}
	if (! this->bottomRightTitle.empty())
	{
		int x = (this->getW() - 1);
		int w = this->bottomRightTitle.size();

		this->print(this->bottomRightTitle,
		            x - w, this->getH() - 1,
		            Colors::pair(COLOR_BLUE, COLOR_DEFAULT));
	}
}
int Window::getW() const
{
	return this->width;
}
int Window::getH() const
{
	return this->height;
}
int Window::getX() const
{
	return this->x;
}
int Window::getY() const
{
	return this->y;
}
void Window::borders(BorderType type)
{
	this->borderType = type;

	if (type == Window::BORDER_NONE)
		return;

	if (type == Window::BORDER_FANCY)
	{
		wborder(this->win,
		        ACS_VLINE    | Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
		        ACS_VLINE    | Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true),
		        ACS_HLINE    | Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
		        ACS_HLINE    | Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true),
		        ACS_ULCORNER | Colors::pair(COLOR_WHITE, COLOR_DEFAULT, true),
		        ACS_URCORNER | Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
		        ACS_LLCORNER | Colors::pair(COLOR_WHITE, COLOR_DEFAULT),
		        ACS_LRCORNER | Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));
	}
	else if (type == Window::BORDER_REGULAR)
	{
		wattrset(this->win, Colors::pair(COLOR_BLACK, COLOR_DEFAULT, true));
		wborder(this->win, '|', '|', '-', '-', '+', '+', '+', '+');
	}
}
void Window::horizontalLine(int x, int y, int c, int width, ColorPair pair)
{
	Colors::pairActivate(this->win, pair);
	mvwhline(this->win, y, x, c, width);
}
void Window::setTitle(std::string title, WindowTitlePosition position)
{
	switch (position)
	{
	case TOP_LEFT:     this->topLeftTitle     = title; break;
	case TOP_RIGHT:    this->topRightTitle    = title; break;
	case BOTTOM_LEFT:  this->bottomLeftTitle  = title; break;
	case BOTTOM_RIGHT: this->bottomRightTitle = title; break;
	default: return;
	}
}

