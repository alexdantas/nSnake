#include "Shapes.hpp"

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}

Rectangle::Rectangle(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;

    this->centerX = x + (w/2);
    this->centerY = y + (h/2);
}

Circle::Circle(int x, int y, int r)
{
    this->x = x;
    this->y = y;
    this->r = r;

    this->centerX = x + r;
    this->centerY = y + r;
}

