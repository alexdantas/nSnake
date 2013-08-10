#include "Window.hpp"

Window::Window(int x, int y, int w, int h)
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
}
WINDOW* Window::getWin()
{
    return (this->win);
}

