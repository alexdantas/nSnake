#ifndef BOARDPARSER_H_DEFINED
#define BOARDPARSER_H_DEFINED

#include <Game/Board.hpp>

/// Hard-coded limit of players that can appear at once on a level.
#define MAX_NUMBER_OF_PLAYERS 1

/// Opens, loads and parses a level file, returning a
/// well-formed `Board`.
///
class BoardParser
{
public:
    /// Loads and parses the level at *filename*.
    ///
    /// @return A new `Board` if successful, NULL if failed.
    /// @note Make sure to delete it later!
    ///
    static Board* load(std::string filename);

    /// TODO
	bool save(Board* board, std::string filename);


	/// THE FOLLOWING STUFF SHOULD GO TO `BOARD`
    /// I'LL MAKE THE TRANSITION ONE METHOD AT A TIME

    // int getWidth();
    // int getHeight();

    // /// Returns the *BoardParserContent* on *x* *y*.
    // Tile::TileContents& at(int x, int y);

    // /// Gets starting x of *player*.
    // ///
    // /// @return 0 if the player is invalid.
    // int getStartingX(int player);

    // /// Gets starting x of *player*.
    // ///
    // /// @return 0 if the player is invalid.
    // int getStartingY(int player);

    // /// Returns how many players the current level supports.
    // int getPlayers();

    // // returning level metadata

    // std::string getName();
    // std::string getAuthor();
    // std::string getDate();
    // std::string getComment();
};

#endif //BOARDPARSER_H_DEFINED

