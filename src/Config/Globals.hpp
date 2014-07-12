#ifndef GLOBALS_H_DEFINED
#define GLOBALS_H_DEFINED

#include <Interface/Colors.hpp>
#include <Game/Score.hpp>

#include <string>

/// All global settings to the game.
///
namespace Globals
{
	/// Allocates necessary variables.
	///
	/// @note No need for Globals::exit() because the
	///       Operational System always frees the memory
	///       when quitting the program.
	///       And definitely this module will need to
	///       be accessed until the end of the program.
	///
	void init();

	/// Warns the user about any errors and warnings found
	/// during the program's execution.
	///
	/// @note You must call this _after_ finishing up nCurses,
	///       otherwise things will get messed up on the terminal.
	void exit();

	/// Loads configuration from the default file name.
	void loadFile();

	/// Saves current configurations to the default file name.
	void saveFile();

	// Accessing version numbers - version[MAJOR] for example
#define MAJOR 0
#define MINOR 1
#define PATCH 2

	/// Game version (format MMP - Major Minor Patch)
	extern int version[3];

	namespace Config
	{
		/// Root directory where we place configurations.
		///
		/// It's `~/.local/share/nsnake/`.
		///
		/// @note It has a trailing '/'.
		extern std::string directory;

		/// Main file where global settings reside.
		///
		/// It's `~/.local/share/nsnake/settings.yml`.
		extern std::string file;

		/// Main file where the high scores are located.
		///
		/// It's `~/.local/share/nsnake/scores.bin`.
		extern std::string scoresFile;
	};

	namespace Screen
	{
		extern bool center_horizontally;
		extern bool center_vertically;

		extern bool show_borders;
		extern bool fancy_borders;
		extern bool outer_border;
	};

	namespace Game
	{
		extern unsigned int starting_level;

		extern int fruits_at_once;
		extern bool random_walls;
		extern bool teleport;

		// The board size
		enum BoardSize
		{
			SMALL, MEDIUM, LARGE
		};
		BoardSize intToBoardSize(int val);
		int boardSizeToInt(BoardSize size);

		extern BoardSize board_size;

		/// Maximum high score obtained for the current game.
		///
		/// There's different high scores for different
		/// game settings.
		///
		/// Each time the user runs the game with a different
		/// configuration of the tweaks above, a new high
		/// score is generated.
		///
		/// It always starts with 0 and if the player
		/// surpasses it, will be the new maximum.
		extern Score highScore;
	};

	namespace Theme
	{
		extern ColorPair text;
		extern ColorPair hilite_text;
		extern ColorPair textbox;
	};

	// Flags to warn the user of some error at the end
	// of execution.
	namespace Error
	{
		/// We could create config file.
		extern bool has_config_file;

		/// We could create score file.
		extern bool has_score_file;

		/// Incompatible score file.
		extern bool old_version_score_file;

		/// Bad-formatted score file.
		extern bool strange_score_file;
	};
};

#endif //GLOBALS_H_DEFINED

