#include "Shapes.hpp"

Point::Point(int x, int y):
    x(x),
    y(y)
{ }

Point::Point():
    x(0),
    y(0)
{ }

Rectangle::Rectangle():
    x(0), y(0), w(1), h(1)
{ }

Rectangle::Rectangle(int x, int y, int w, int h):
    x(x), y(y), w(w), h(h)
{
    this->center = Point(x + (w/2), y + (h/2));
}

Circle::Circle(int x, int y, int r)
{
    this->x = x;
    this->y = y;
    this->r = r;

    this->centerX = x + r;
    this->centerY = y + r;
}

