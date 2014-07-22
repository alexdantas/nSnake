#ifndef BOARDPARSER_H_DEFINED
#define BOARDPARSER_H_DEFINED

#include <Game/Board.hpp>

#define COMMENT_CHAR ';'
#define WALL_CHAR    '#'
#define SNAKE_CHAR   '@'

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
};

#endif //BOARDPARSER_H_DEFINED

