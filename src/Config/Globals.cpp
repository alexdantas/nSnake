#include <Config/Globals.hpp>
#include <Config/YAMLFile.hpp>
#include <Misc/Utils.hpp>
#include <Flow/InputManager.hpp>

#include <ncurses.h>
#include <iostream>
#include <fstream>

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
	Globals::Theme::text        = 0;
	Globals::Theme::hilite_text = Colors::pair(COLOR_CYAN, COLOR_DEFAULT);
	Globals::Theme::textbox     = (Globals::Theme::hilite_text | A_REVERSE);

	// Making sure default config directory exists
	// By default it's `~/.local/share/nsnake/`

	Globals::Config::directory = (Utils::File::getHome() +
	                              ".local/share/" +
	                              PACKAGE + "/");

	if (Utils::String::front(Globals::Config::directory) != '/')
	{
		// We couldn't get user's home directory,
		// so let's fallback to `/tmp/.local/share...`
		Globals::Config::directory = ("/tmp/" +
		                              Globals::Config::directory);
	}

	Globals::Config::file = (Globals::Config::directory +
	                         "settings.yml");

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

	// Default Input configurationa
	InputManager::bind("left",  KEY_LEFT);
	InputManager::bind("right", KEY_RIGHT);
	InputManager::bind("up",    KEY_UP);
	InputManager::bind("down",  KEY_DOWN);
	InputManager::bind("pause", 'p');
	InputManager::bind("help",  'h');
	InputManager::bind("quit",  'q');
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
	// DEPRECATED: Adding support for old .ini configuration files
	//             The following prepends the .ini file with a warning
	//
	// I know I should convert from .ini to .yml but I'm wicked.
	//
	// NOTE: Remove this a few versions from now...
	//
	if (Utils::File::exists(Globals::Config::directory + "settings.ini"))
	{
		std::ifstream old_file_in((Globals::Config::directory + "settings.ini").c_str());

		std::stringstream buffer;
		buffer << old_file_in.rdbuf();
		old_file_in.close();

		// To avoid prepending this thing over and over, we must
		// check if it already exists
		std::string tmp;
		std::getline(buffer, tmp);
		if (tmp.find("# Warning:") == std::string::npos)
		{
			std::ofstream old_file_out((Globals::Config::directory + "settings.ini").c_str());

			old_file_out << "# Warning: this .ini file was deprecated in favor\n";
			old_file_out << "#          of the new .yml format. You can safely\n";
			old_file_out << "#          delete it.\n";
			old_file_out << buffer.rdbuf();
			old_file_out.close();
		}
	}

	// Now, back on track
	if (! Utils::File::exists(Globals::Config::file))
		return;

	YAMLFile yaml;
	try {
		yaml.load(Globals::Config::file);
	}
	catch(YAML::BadFile& e)
	{
		// File doesn't exist (or we couldn't access it)
		// Either way, ignore it silently
		return;
	}

// Small macro to avoid unnecessary typing.
//
// To get something from the ini file we send the
// text (to identify some value) and the default
// value in case it doesn't exist.
//
// For the last one I send the variable itself,
// so we fallback to the default values.
#define YAML_GET(var, out, in)     \
	{                              \
		var = yaml.get(out, in, var); \
	}

	YAML_GET(Globals::Screen::center_horizontally, "screen", "center_horizontal");
	YAML_GET(Globals::Screen::center_vertically,   "screen", "center_vertical");

	YAML_GET(Globals::Screen::show_borders,  "screen", "borders");
	YAML_GET(Globals::Screen::fancy_borders, "screen", "fancy_borders");
	YAML_GET(Globals::Screen::outer_border,  "screen", "outer_border");

	YAML_GET(Globals::Game::random_walls,   "game", "random_walls");
	YAML_GET(Globals::Game::fruits_at_once, "game", "fruits_at_once");
	YAML_GET(Globals::Game::teleport,       "game", "teleport");

	// unsigned ints are the exception - their overloading
	// is ambiguous... I should consider removing them altogether
	Globals::Game::starting_level = yaml.get("game", "starting_level", (int)Globals::Game::starting_level);

	// Special Cases

	// Getting input keys
	std::string tmp;

	YAML_GET(tmp, "input", "left");
	InputManager::bind("left", InputManager::stringToKey(tmp));

	YAML_GET(tmp, "input", "right");
	InputManager::bind("right", InputManager::stringToKey(tmp));

	YAML_GET(tmp, "input", "up");
	InputManager::bind("up", InputManager::stringToKey(tmp));

	YAML_GET(tmp, "input", "down");
	InputManager::bind("down", InputManager::stringToKey(tmp));

	YAML_GET(tmp, "input", "pause");
	InputManager::bind("pause", InputManager::stringToKey(tmp));

	YAML_GET(tmp, "input", "help");
	InputManager::bind("help", InputManager::stringToKey(tmp));

	YAML_GET(tmp, "input", "quit");
	InputManager::bind("quit", InputManager::stringToKey(tmp));

	// Board Size
	int board_size = 2;
	YAML_GET(board_size, "game", "board_size");
	Globals::Game::board_size = Globals::Game::intToBoardSize(board_size);
}
void Globals::saveFile()
{
	// Even if the file doesn't exist, we'll create it.
	YAMLFile yaml;
	try
	{
		yaml.load(Globals::Config::file);
	}
	catch(YAML::BadFile& e)
	{
		yaml.create();
	}

// Other macro to avoid typing, similar to the one
// at loadFile()
#define YAML_SET(out, in, var)	\
	{                           \
		yaml.set(out, in, var); \
	}


	YAML_SET("screen", "center_horizontal", Globals::Screen::center_horizontally);
	YAML_SET("screen", "center_vertical",   Globals::Screen::center_vertically);

	YAML_SET("screen", "borders",       Globals::Screen::show_borders);
	YAML_SET("screen", "fancy_borders", Globals::Screen::fancy_borders);
	YAML_SET("screen", "outer_border",  Globals::Screen::outer_border);

	YAML_SET("game", "random_walls",     Globals::Game::random_walls);
	YAML_SET("game", "fruits_at_once",   Globals::Game::fruits_at_once);
	YAML_SET("game", "teleport",         Globals::Game::teleport);

	// unsigned ints are the exception - their overloading
	// is ambiguous... I should consider removing them altogether
	yaml.set("game", "starting_level", (int)Globals::Game::starting_level);

	// Special Cases

	// Input Keys
	std::string key;

	key = InputManager::keyToString(InputManager::getBind("left"));
	YAML_SET("input", "left", key);

	key = InputManager::keyToString(InputManager::getBind("right"));
	YAML_SET("input", "right", key);

	key = InputManager::keyToString(InputManager::getBind("up"));
	YAML_SET("input", "up", key);

	key = InputManager::keyToString(InputManager::getBind("down"));
	YAML_SET("input", "down", key);

	key = InputManager::keyToString(InputManager::getBind("pause"));
	YAML_SET("input", "pause", key);

	key = InputManager::keyToString(InputManager::getBind("help"));
	YAML_SET("input", "help", key);

	key = InputManager::keyToString(InputManager::getBind("quit"));
	YAML_SET("input", "quit", key);

	// Board size
	int board_size = Globals::Game::boardSizeToInt(Globals::Game::board_size);
	YAML_SET("game", "board_size", board_size);

	try
	{
		yaml.save(Globals::Config::file);
	}
	catch(std::runtime_error)
	{
		// Couldn't save the file...
		// Silently return
		return;
	}
}

