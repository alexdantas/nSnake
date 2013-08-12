#include "Sprite.hpp"
#include "Ncurses.hpp"

Sprite::Sprite(std::string sprite, int width, int height, unsigned long color):
    sprite(sprite),
    width(width),
    height(height),
    color(color)
{ }
void Sprite::render(int x, int y)
{
    Ncurses::setStyle(this->color);
    Ncurses::print(this->sprite, x, y);
}
void Sprite::setStyle(unsigned long style)
{
    this->color = style;
}

