#ifndef GLOBALS_H_DEFINED
#define GLOBALS_H_DEFINED

#include <Interface/Colors.hpp>

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
		/// It's `~/.local/share/nsnake/settings.ini`.
		///
		/// @note Most settings are user-wide, according to
		///       it's Profile.
		extern std::string file;
	};

	namespace Screen
	{
		extern bool center_horizontally;
		extern bool center_vertically;

		extern bool show_borders;
		extern bool fancy_borders;
		extern bool outer_border;

		extern bool use_colors;
	};

	namespace Game
	{
		extern unsigned int starting_level;

		extern std::string random_algorithm;

		extern bool has_game_over_animation;

		extern bool random_mode;
	};

	namespace Theme
	{
		extern ColorPair text;
		extern ColorPair hilite_text;
		extern ColorPair textbox;
	};

	namespace Input
	{
		extern int left;
		extern int right;
		extern int up;
		extern int down;
		extern int pause;
		extern int help;
		extern int quit;
	};
};

#endif //GLOBALS_H_DEFINED

