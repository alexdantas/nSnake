#ifndef BOARDPARSER_H_DEFINED
#define BOARDPARSER_H_DEFINED

#include <Entities/Board.hpp>

#include <exception>
#include <string>

/// Custom exception class to specify an error that
/// occurred during a level loading.
///
class BoardParserException : public std::exception
{
public:
	BoardParserException(std::string message):
		message(message)
	{ }
	~BoardParserException() throw()
	{ }

	std::string message;
};

#define COMMENT_CHAR ';'
#define WALL_CHAR    '#'
#define SNAKE_CHAR   '@'

/// Opens, loads and parses a level file, returning a
/// well-formed `Board`.
///
class BoardParser
{
public:
	/// Default directory where the level files are.
	///
	/// @note Defaults to `Globals::Config::directory + levels/`,
	///       which at the time of writing is `~/.local/share/nsnake/levels`
	static std::string directory;

	/// Default extension for nSnake level files.
	///
	/// It's all the part that comes _after the dot_ on
	/// a file name.
	///
	/// @note Defaults to "nsnake"
	/// @note Files that does not end with it will be _ignored_.
	static std::string extension;

    /// Loads and parses level with *name*.
    ///
    /// @note It looks for it on standard levels location,
    ///       whatever that might mean.
    ///
    /// @see loadFile
    ///
    static Board* load(std::string filename);

    /// Loads and parses the level at *filename*.
    ///
    /// @return A new `Board` if successful, NULL if failed.
    /// @note Make sure to delete it later!
    ///
    static Board* loadFile(std::string filename);

    /// TODO
	static bool save(Board* board, std::string filename);

	/// Lists all levels found by the game.
	///
	/// It looks on standard level locations.
	/// Whatever that might mean.
	///
	static std::vector<std::string> listLevels();
};

#endif //BOARDPARSER_H_DEFINED

