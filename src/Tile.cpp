#include "Tile.hpp"
#include "Ncurses.hpp"

Tile::Tile():
    sprite(NULL),
    content(Tile::NOTHING)
{ }
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
    case Tile::NOTHING:
        break;

    case Tile::FOOD:
        this->sprite = new Sprite("F", 1, 1, Color::pair("red", "black"));
        break;

    case Tile::SNAKE_HEAD:
        this->sprite = new Sprite("@", 1, 1, Color::pair("green", "black", "bold"));
        break;

    case Tile::SNAKE:
        this->sprite = new Sprite("o", 1, 1, Color::pair("green", "black", "bold"));
        break;

    default: // invalid shit
        this->content = Tile::NOTHING;
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
    if (!(this->isEmpty()))
        this->sprite->render(x, y);
}
bool Tile::isEmpty()
{
    return ((this->content == Tile::NOTHING) &&
            (this->sprite  == NULL));
}

