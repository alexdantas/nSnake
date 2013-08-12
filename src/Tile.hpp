#ifndef TILE_H_DEFINED
#define TILE_H_DEFINED

#include "Sprite.hpp"

///
class Tile
{
public:

    enum TileContents
    {
        NOTHING, BORDER, WALL, FOOD, SNAKE_HEAD, SNAKE
    };

    Tile();
    virtual ~Tile();

    void set(TileContents newContent);

    void clear();

    void render(int x, int y);

    bool isEmpty();

private:
    Sprite* sprite;
    TileContents content;
};

#endif //TILE_H_DEFINED

