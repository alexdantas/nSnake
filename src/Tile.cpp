#include "Tile.hpp"
#include "Ncurses.hpp"

Tile::Tile():
    sprite(NULL),
    spriteIndex(Tile::NOTHING)
{
    this->clear();
}
Tile::~Tile()
{
    if (this->sprite)
        delete this->sprite;
}
void Tile::set(TileContents newContent)
{
    this->clear();
    this->add(newContent);
}
void Tile::clear()
{
    // I suffered a lot here
    // std::vector::resize only sets default values
    // if the size is greater than it currently is

    this->content.resize(TILE_CONTENTS_MAX);
    for (int i = 0; i < (TILE_CONTENTS_MAX); i++)
        this->content[i] = false;

    this->content[Tile::NOTHING] = true;

    this->spriteIndex = Tile::NOTHING;
    this->spriteRefresh();
}
void Tile::add(TileContents newContent)
{
    if (this->has(newContent))
        return;

    this->content[Tile::NOTHING] = false;
    this->content[newContent] = true;

    this->spriteIndex = newContent;
    this->spriteRefresh();
}
void Tile::remove(TileContents newContent)
{
    if (!(this->has(newContent)))
        return;

    this->content[newContent] = false;

    if (this->isEmpty())
    {
        this->clear();
        return;
    }

    // If the sprite is the thing we'll remove right now,
    // let's look for other thing to print
    if (this->spriteIndex == newContent)
    {
        for (unsigned int i = 0; i < (TILE_CONTENTS_MAX); i++)
        {
            if (this->content[i])
            {
                this->spriteIndex = (TileContents)(i);
                this->spriteRefresh();
                break;
            }
        }
    }
}
bool Tile::has(TileContents newContent)
{
    return (this->content[newContent]);
}
void Tile::render(int x, int y)
{
    if (this->isEmpty())
        return;

    if (this->sprite)
    this->sprite->render(x, y);
}
bool Tile::isEmpty()
{
    for (int i = 0; i < TILE_CONTENTS_MAX; i++)
        if ((this->content[i]) && (i != Tile::NOTHING))
            return false;

    return true;
}
void Tile::spriteRefresh()
{
    if (this->sprite)
    {
        delete this->sprite;
        this->sprite = NULL;
    }

    switch (this->spriteIndex)
    {
    case Tile::NOTHING:
        break;

    case Tile::BORDER:
        this->sprite = new Sprite("#", Color::pair("white", "default"));
        break;

    case Tile::TELEPORT_BORDER:
        this->sprite = new Sprite(".", Color::pair("white", "default"));
        break;

    case Tile::WALL:
        this->sprite = new Sprite("#", Color::pair("white", "default", "bold"));
        break;

    case Tile::FOOD:
        this->sprite = new Sprite("$", Color::pair("yellow"));
        break;

    case Tile::SNAKE_HEAD:
        this->sprite = new Sprite("@", Color::pair("green", "default", "bold"));
        break;

    case Tile::SNAKE_DEAD_HEAD:
        this->sprite = new Sprite("x", Color::pair("red"));
        break;

    case Tile::SNAKE_BODY:
        this->sprite = new Sprite("o", Color::pair("green"));
        break;

    default: // invalid shit
        this->clear();
        break;
    }
}

