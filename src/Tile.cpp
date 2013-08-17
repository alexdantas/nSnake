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

    // If the current sprite was the thing we just removed
    // we've got a problem!
    //
    // Let's get the sprite of the other thing this tile has.

    if (this->spriteIndex == newContent)
    {
        for (int i = 0; i < (TILE_CONTENTS_MAX); i++)
        {
            if (this->content[i]) // A-ha!
            {
                this->spriteIndex = (TileContents)(i); // ugh...
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
    // There are two cases on which a tile is empty.
    //
    // * All of it's contents are false.
    // * All of it's contents are false except Tile::NOTHING.

    for (int i = 0; i < TILE_CONTENTS_MAX; i++)
        if ((this->content[i]) && (i != Tile::NOTHING))
            return false;

    return true;
}
bool Tile::isInvalidForPlayer()
{
    // The thing is...
    // It's kinda hard to determine what's invalid for
    // a player to step into.
    // It depends on the circumstances.
    // For example, sometimes you'd want it to step over
    // a TELEPORT_BORDER, sometimes not.

    if (this->has(Tile::WALL) ||
        this->has(Tile::BORDER) ||
        this->has(Tile::TELEPORT_BORDER) ||
        this->has(Tile::SNAKE_BODY) ||
        this->has(Tile::SNAKE_HEAD) ||
        this->has(Tile::SNAKE_DEAD_HEAD))
        return true;

    return false;
}
void Tile::spriteRefresh()
{
    // Since this will overwrite the current sprite, let's
    // erase the current one.
    // Note that this is the ONLY place (except on the destructor)
    // where I explicitly use delete!

    if (this->sprite)
    {
        delete this->sprite;
        this->sprite = NULL;
    }

    switch (this->spriteIndex)
    {
    case Tile::NOTHING:
        // Keep it NULL
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
bool Tile::isBorder()
{
    return (this->has(Tile::BORDER) ||
            this->has(Tile::TELEPORT_BORDER));
}

