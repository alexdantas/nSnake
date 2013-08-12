#ifndef TILE_H_DEFINED
#define TILE_H_DEFINED

#include "Sprite.hpp"

///
class Tile
{
public:

    enum TileContents
    {
        NOTHING, // Empty tile
        BORDER,  // Solid border around Board, player dies if hit
        TELEPORT_BORDER, // Soft border, Player teleports if hit
        WALL, // Solid wall inside the Board, player dies if hit
        FOOD,
        SNAKE_HEAD,
        SNAKE_DEAD_HEAD, // Appears where the snake has died
        SNAKE_BODY,
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

