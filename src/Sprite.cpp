#include "Sprite.hpp"
#include "Ncurses.hpp"

Sprite::Sprite(std::string sprite, int width, int height):
    sprite(sprite),
    width(width),
    height(height)
{ }
void Sprite::draw(int x, int y)
{
    Ncurses::print(this->sprite, x, y);
}

