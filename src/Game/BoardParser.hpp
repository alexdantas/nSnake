#ifndef BOARDPARSER_H_DEFINED
#define BOARDPARSER_H_DEFINED

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
class BoardParser
{
public:
    /// Creates an empty level.
    ///
    /// I suggest you to call *load()* or something.
    BoardParser();

    virtual ~BoardParser();

    /// Loads and parses the level at *filename*.
    ///
    /// @note If called multiple times, it erases the
    ///       previous loaded levels!
    bool load(std::string filename);

    /// Reloads the current level.
    bool reload();

    /// TODO
    bool save(std::string filename);

    /// Erases the current level into a blank one.
    void clear();

    int getWidth();
    int getHeight();

    /// Returns the *BoardParserContent* on *x* *y*.
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

    // returning level metadata

    std::string getName();
    std::string getAuthor();
    std::string getDate();
    std::string getComment();

private:

    /// The filename, if properly loaded.
    ///
    /// @note If *load()* is not called, this is empty.
    std::string filename;

    /// The raw contents of the level in string format, if
    /// properly loaded.
    ///
    /// @note If *load()* is not called, this is empty.
    std::vector<std::string> rawBoardParser;

    /// The actual level, in respect of *TileContents*.
    std::vector<std::vector<Tile::TileContents> > level;

    /// BoardParser's maximum width.
    ///
    /// @note All lines are guaranteed to have this width.
    ///       independently if the map file is not regular.
    int width;

    /// BoardParser's height.
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

#endif //BOARDPARSER_H_DEFINED

