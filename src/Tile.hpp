#ifndef TILE_H_DEFINED
#define TILE_H_DEFINED

#include "Sprite.hpp"

///
class Tile
{
public:

    enum TileContents
    {
        NOTHING, FOOD
    };

    Tile();
    virtual ~Tile();

    void set(TileContents newContent);
    void clear();
    void render(int x, int y);

private:
    Sprite* sprite;
    TileContents content;
};

#endif //TILE_H_DEFINED

