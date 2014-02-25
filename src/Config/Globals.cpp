#include <Config/Globals.hpp>
#include <Config/INI.hpp>
#include <Misc/Utils.hpp>

#include <ncurses.h>
#include <iostream>

// VERSION is formatted like "0.0.1" - i'm skipping the dots
int Globals::version[3] = { VERSION[0] - '0',
                            VERSION[2] - '0',
                            VERSION[4] - '0'};

//  __    ___   _      ____  _   __
// / /`  / / \ | |\ | | |_  | | / /`_
// \_\_, \_\_/ |_| \| |_|   |_| \_\_/

// real initialization at init()
std::string Globals::Config::directory  = "";
std::string Globals::Config::file       = "";
std::string Globals::Config::scoresFile = "";

bool Globals::Screen::center_horizontally = true;
bool Globals::Screen::center_vertically   = true;

bool Globals::Screen::show_borders  = true;
bool Globals::Screen::fancy_borders = true;
bool Globals::Screen::outer_border  = true;

unsigned int Globals::Game::starting_level          = 1;
int          Globals::Game::fruits_at_once          = 1;
bool         Globals::Game::random_walls            = false;
bool         Globals::Game::teleport                = false;

Globals::Game::BoardSize Globals::Game::board_size  = LARGE;

Globals::Game::BoardSize Globals::Game::intToBoardSize(int val)
{
	if (val == 0)
		return Globals::Game::SMALL;

	if (val == 1)
		return Globals::Game::MEDIUM;

	return Globals::Game::LARGE;
}
int Globals::Game::boardSizeToInt(Globals::Game::BoardSize size)
{
	if (size == Globals::Game::SMALL)
		return 0;

	if (size == Globals::Game::MEDIUM)
		return 1;

	return 2;
}

Score        Globals::Game::highScore;

ColorPair Globals::Theme::text;
ColorPair Globals::Theme::hilite_text;
ColorPair Globals::Theme::textbox;

int Globals::Input::left  = KEY_LEFT;
int Globals::Input::right = KEY_RIGHT;
int Globals::Input::up    = KEY_UP;
int Globals::Input::down  = KEY_DOWN;
int Globals::Input::pause = 'p';
int Globals::Input::help  = 'h';
int Globals::Input::quit  = 'q';

bool Globals::Error::has_config_file = true;
bool Globals::Error::has_score_file  = true;
bool Globals::Error::old_version_score_file = false;
bool Globals::Error::strange_score_file     = false;

//  _   _      _  _____
// | | | |\ | | |  | |
// |_| |_| \| |_|  |_|

void Globals::init()
{
	// Other default variables
	Globals::Theme::text = 0;
	Globals::Theme::hilite_text = Colors::pair(COLOR_CYAN, COLOR_DEFAULT);
	Globals::Theme::textbox = Globals::Theme::hilite_text | A_REVERSE;

	// Making sure default config directory exists
	// By default it's `~/.local/share/nsnake/`

	Globals::Config::directory = (Utils::File::getHome() +
	                              ".local/share/" +
	                              PACKAGE + "/");

	if (Globals::Config::directory.front() != '/')
	{
		// We couldn't get user's home directory,
		// so let's fallback to `/tmp/.local/share...`
		Globals::Config::directory = ("/tmp/" +
		                              Globals::Config::directory);
	}

	Globals::Config::file = (Globals::Config::directory +
	                         "settings.ini");

	Globals::Config::scoresFile = (Globals::Config::directory +
	                               "scores.bin");

	if (! Utils::File::isDirectory(Globals::Config::directory))
		Utils::File::mkdir_p(Globals::Config::directory);

	if (! Utils::File::isDirectory(Globals::Config::directory))
	{
		// We REALLY can't access the disk by any means.
		// Let's throw everything away and give up.
		Globals::Config::directory = "/dev/";
		Globals::Config::file      = "/dev/null";
		return;
	}
}
void Globals::exit()
{
	if (! Globals::Error::has_config_file)
	{
		std::cout << "Warning: We could not create the configuration file.\n"
		          << "         Please check permissions to the path:\n"
		          << "         " + Globals::Config::file
		          << std::endl;
	}
	if (! Globals::Error::has_score_file)
	{
		std::cout << "Warning: We could not create the score file.\n"
		          << "         Please check permissions to the path:\n"
		          << "         " + Globals::Config::scoresFile
		          << std::endl;
	}
	if (Globals::Error::old_version_score_file)
	{
		std::cout << "Warning: Your high score file is from an old nsnake version."
		          << std::endl;
	}
	if (Globals::Error::strange_score_file)
	{
		// Erasing high scores...
		Utils::File::create(Globals::Config::scoresFile);

		std::cout << "Error: Corrupted high score file!\n"
		          << "       We're sorry, but we had to erase it"
		          << std::endl;
	}
}
void Globals::loadFile()
{
	if (! Utils::File::exists(Globals::Config::file))
		return;

	INI ini;
	if (! ini.load(Globals::Config::file))
		return;

// Small macro to avoid unnecessary typing.
//
// To get something from the ini file we send the
// text (to identify some value) and the default
// value in case it doesn't exist.
//
// For the last one I send the variable itself,
// so we fallback to the default values.
#define INI_GET(var, text) \
	{ \
		var = ini.get(text, var); \
	}

	INI_GET(Globals::Screen::center_horizontally, "screen:center_horizontal");
	INI_GET(Globals::Screen::center_vertically,   "screen:center_vertical");

	INI_GET(Globals::Screen::show_borders,  "screen:borders");
	INI_GET(Globals::Screen::fancy_borders, "screen:fancy_borders");
	INI_GET(Globals::Screen::outer_border,  "screen:outer_border");

	INI_GET(Globals::Game::starting_level, "game:starting_level");
	INI_GET(Globals::Game::random_walls,   "game:random_walls");
	INI_GET(Globals::Game::fruits_at_once, "game:fruits_at_once");
	INI_GET(Globals::Game::teleport,       "game:teleport");

	int board_size = 2;
	INI_GET(board_size, "game:board_size");
	Globals::Game::board_size = Globals::Game::intToBoardSize(board_size);
}
void Globals::saveFile()
{
	// Even if the file doesn't exist, we'll create it.
	INI ini;
	if (! ini.load(Globals::Config::file))
		ini.create();

// Other macro to avoid typing, similar to the one
// at loadFile()
#define INI_SET(text, var) \
	{ \
		ini.set(text, Utils::String::toString(var)); \
	}


	INI_SET("screen:center_horizontal", Globals::Screen::center_horizontally);
	INI_SET("screen:center_vertical",   Globals::Screen::center_vertically);

	INI_SET("screen:borders",       Globals::Screen::show_borders);
	INI_SET("screen:fancy_borders", Globals::Screen::fancy_borders);
	INI_SET("screen:outer_border",  Globals::Screen::outer_border);

	INI_SET("game:starting_level",   Globals::Game::starting_level);
	INI_SET("game:random_walls",     Globals::Game::random_walls);
	INI_SET("game:fruits_at_once",   Globals::Game::fruits_at_once);
	INI_SET("game:teleport",         Globals::Game::teleport);

	int board_size = Globals::Game::boardSizeToInt(Globals::Game::board_size);
	INI_SET("game:board_size", board_size);

	ini.save(Globals::Config::file);
}

