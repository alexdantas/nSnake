#ifndef LEVEL_H_DEFINED
#define LEVEL_H_DEFINED

#include <string>
#include <vector>
#include "Tile.hpp"

/// Single level of the game.
///
/// Loads levels from files and stores them in a way
/// that Board can build them.
class Level
{
public:
    Level();
    virtual ~Level();

    /// Loads and parses the level at *filename*.
    ///
    /// @note If called multiple times, it erases the
    ///       previous loaded levels!
    bool load(std::string filename);

    /// TODO
    bool save(std::string filename);

    /// Erases the current level into a blank one.
    void clear();

    int getWidth();
    int getHeight();

    /// Returns the *LevelContent* on *x* *y*.
    Tile::TileContents& at(int x, int y);

    int getStartingX();
    int getStartingY();

private:
    /// The level's loaded filename.
    std::string filename;

    /// The raw contents of the level in string format.
    std::vector<std::string> rawLevel;

    /// The actual level, in respect of *TileContents*.
    std::vector<std::vector<Tile::TileContents> > level;

    /// Level's maximum width.
    ///
    /// @note Not all lines may have this same width!
    int width;

    /// Level's height.
    int height;

    int startingX; /// X position on where the player should start.
    int startingY; /// Y position on where the player should start.

    // optional metadata on the level

    std::string name;
    std::string author;
    std::string date;
    std::string comment;
};

#endif //LEVEL_H_DEFINED

