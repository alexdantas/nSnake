#ifndef LEVEL_H_DEFINED
#define LEVEL_H_DEFINED

#include <string>
#include <vector>
#include "Tile.hpp"
#include "Shapes.hpp"

/// Hard-coded limit of players that can appear at once on a level.
#define MAX_NUMBER_OF_PLAYERS 4

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

    /// Gets starting x of *player*.
    ///
    /// @return 0 if the player is invalid.
    int getStartingX(int player);

    /// Gets starting x of *player*.
    ///
    /// @return 0 if the player is invalid.
    int getStartingY(int player);

    /// Returns how many players the current level supports.
    int getPlayers();

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

    /// How many players are supported by this map?
    ///
    /// @note We have a hard-coded limit of players.
    int players;

    /// All the starting positions for the players.
    ///
    /// @note We have a hard-coded limit of players.
    Point startingPosition[MAX_NUMBER_OF_PLAYERS];

    // optional metadata on the level

    std::string name;
    std::string author;
    std::string date;
    std::string comment;
};

#endif //LEVEL_H_DEFINED

