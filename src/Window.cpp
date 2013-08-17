#include "Window.hpp"
#include "Color.hpp"

Window::Window(int x, int y, int w, int h):
    hasBorder(false)
{
    this->box = new Rectangle(x, y, w, h);

    this->win = newwin(h, w, y, x);
    if (!this->win)
        this->error = true;

    // Support for extra keys (arrow keys, F1, F2, ... )
	keypad(this->win, TRUE);

    this->usingExternalBox = false;
}
Window::Window(Rectangle* rect)
{
    this->box = rect;

    this->win = newwin(this->box->h, this->box->w,
                       this->box->y, this->box->x);
    if (!this->win)
        this->error = true;

    this->usingExternalBox = true;
}
Window::~Window()
{
    if (this->win) delwin(this->win);
    if (this->box) delete this->box;
}
// Window::move(int x, int y)
// {

// }
void Window::resize(int w, int h)
{
    wresize(this->win, h, w);
    this->box->w = w;
    this->box->h = h;
}
void Window::print(std::string str, int x, int y)
{
    mvwaddstr(this->win, y, x, str.c_str());
}
void Window::setStyle(unsigned long pair)
{
    wattrset(this->win, pair);
}
void Window::setBackground(chtype ch, unsigned long pair)
{
    wbkgd(this->win, ch | pair);
}
void Window::refresh()
{
    wrefresh(this->win);
}
void Window::clear()
{
    werase(this->win);
    if (this->hasBorder)
    {
        this->setStyle(Color::pair("white"));
        wborder(this->win,
                this->border.left,
                this->border.right,
                this->border.top,
                this->border.bottom,
                this->border.topLeft,
                this->border.topRight,
                this->border.bottomLeft,
                this->border.bottomRight);
    }
}
WINDOW* Window::getWin()
{
    return (this->win);
}
void Window::setBorder()
{
    this->hasBorder = true;

    this->border.left = 0;
    this->border.right = 0;
    this->border.top = 0;
    this->border.bottom = 0;
    this->border.topLeft = 0;
    this->border.topRight = 0;
    this->border.bottomLeft = 0;
    this->border.bottomRight = 0;
}
void Window::setBorder(int horizontal, int vertical, int edges)
{
    this->hasBorder = true;

    this->border.left = horizontal;
    this->border.right = horizontal;
    this->border.top = vertical;
    this->border.bottom = vertical;
    this->border.topLeft = edges;
    this->border.topRight = edges;
    this->border.bottomLeft = edges;
    this->border.bottomRight = edges;
}
void Window::setBorder(int left, int right, int top, int bottom,
                       int topLeft, int topRight, int bottomLeft, int bottomRight)
{
    this->hasBorder = true;

    this->border.left = left;
    this->border.right = right;
    this->border.top = top;
    this->border.bottom = bottom;
    this->border.topLeft = topLeft;
    this->border.topRight = topRight;
    this->border.bottomLeft = bottomLeft;
    this->border.bottomRight = bottomRight;
}

