#include "Sprite.hpp"
#include "Ncurses.hpp"

Sprite::Sprite(std::string sprite, unsigned long color, int width, int height):
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

