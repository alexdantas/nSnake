#ifndef TILE_H_DEFINED
#define TILE_H_DEFINED

#include <vector>
#include "Sprite.hpp"

/// A single tile of the map.
///
/// A *Board* is composed of several of these.
class Tile
{
public:

    /// All possible things that can be inside a tile.
    ///
    /// There can be multiple things inside it, so you should
    /// check it out on your code.
    enum TileContents
    {
        NOTHING, // Empty tile
        BORDER,  // Solid border around Board, player dies if hit
        TELEPORT_BORDER, // Soft border, Player teleports if hit
        WALL, // Solid wall inside the Board, player dies if hit
        FOOD, // Thing that the snake eats and grows
        SNAKE_HEAD, // The head of the snake
        SNAKE_DEAD_HEAD, // Appears where the snake has died
        SNAKE_BODY, // Pieces of the snake body

        TILE_CONTENTS_MAX // This arbitrary value exists so the
                          // content vector can be safely resized
                          // to contain all possible things
    };

    /// Creates an empty tile.
    Tile();

    virtual ~Tile();

    /// Sets the tile to only *newContent*.
    void set(TileContents newContent);

    /// Erases any content that may be inside the tile.
    void clear();

    /// Adds *newContent* to the tile.
    void add(TileContents newContent);

    /// Removes, is possible, *newContent* from the tile.
    void remove(TileContents newContent);

    /// Tells if the tile contains *newContent*.
    bool has(TileContents newContent);

    /// Shows the tile onscreen, based on it's *sprite*.
    void render(int x, int y);

    /// Tells if the tile is empty, has no contents.
    bool isEmpty();

private:
    /// Refreshes the sprite of the tile, based on
    /// *spriteIndex*.
    void spriteRefresh();

    /// The appearance of this tile, how it will be printed.
    Sprite* sprite;

    /// All things that are inside this tile.
    ///
    /// It's a vector of bool, meaning that everything that's
    /// inside this tile is *true* and everything else, *false*.
    std::vector<bool> content;

    /// Which thing is being printed by the sprite.
    TileContents spriteIndex;

//    std::vector<TileContents> content;
};

#endif //TILE_H_DEFINED

