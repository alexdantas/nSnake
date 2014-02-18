#include <Config/Globals.hpp>
#include <Config/INI.hpp>
#include <Misc/Utils.hpp>

#include <ncurses.h>

// VERSION is formatted like "0.0.1" - i'm skipping the dots
int Globals::version[3] = { VERSION[0] - '0',
                            VERSION[2] - '0',
                            VERSION[4] - '0'};

//  __    ___   _      ____  _   __
// / /`  / / \ | |\ | | |_  | | / /`_
// \_\_, \_\_/ |_| \| |_|   |_| \_\_/

// real initialization at init()
std::string Globals::Config::directory = "";
std::string Globals::Config::file      = "";

bool Globals::Screen::center_horizontally = true;
bool Globals::Screen::center_vertically   = true;

bool Globals::Screen::show_borders  = true;
bool Globals::Screen::fancy_borders = true;
bool Globals::Screen::outer_border  = true;

bool Globals::Screen::use_colors = true;

unsigned int Globals::Game::starting_level          = 1;
std::string  Globals::Game::random_algorithm        = "regular";
bool         Globals::Game::has_game_over_animation = true;

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

	// Getting default profile name - should be at the
	// global settings file.
	if (Utils::File::exists(Globals::Config::file))
	{
		INI ini;
		if (ini.load(Globals::Config::file))
		{
			// LOADING SetTINGS
		}
	}
}

