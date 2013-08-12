#include "Tile.hpp"
#include "Ncurses.hpp"

Tile::Tile():
    sprite(NULL),
    content(Tile::NOTHING)
{

}
Tile::~Tile()
{
    if (this->sprite) delete this->sprite;
}
void Tile::set(TileContents newContent)
{
    this->content = newContent;
    if (this->sprite)
    {
        delete this->sprite;
        this->sprite = NULL;
    }

    switch (newContent)
    {
    case NOTHING:
        break;

    case FOOD:
        this->sprite = new Sprite("F", 1, 1, Color::pair("red"));
        break;

    default:
        break;
    }
}
void Tile::clear()
{
    this->content = Tile::NOTHING;
    if (this->sprite)
    {
        delete this->sprite;
        this->sprite = NULL;
    }
}
void Tile::render(int x, int y)
{
    if ((this->content != Tile::NOTHING) &&
        (this->sprite  != NULL))
        this->sprite->render(x, y);
}

