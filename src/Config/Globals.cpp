#include <Config/Globals.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/Helpers/INI.hpp>
#include <Engine/Helpers/File.hpp>
#include <Engine/Helpers/String.hpp>
#include <Engine/InputManager.hpp>
#include <Entities/BoardParser.hpp>
#include <Entities/ScoreFile.hpp>

#include <ncurses.h>
#include <iostream>
#include <fstream>

// VERSION is formatted like "0.0.1" - i'm skipping the dots
char Globals::version[3] = { VERSION[0],
                             VERSION[2],
                             VERSION[4] };

//  __    ___   _      ____  _   __
// / /`  / / \ | |\ | | |_  | | / /`_
// \_\_, \_\_/ |_| \| |_|   |_| \_\_/

// real initialization at init()
std::string Globals::Config::directory  = "";
std::string Globals::Config::file       = "";
std::string Globals::Config::scoresFile = "";

unsigned int Globals::Game::starting_speed          = 1;
int          Globals::Game::fruits_at_once          = 1;
bool         Globals::Game::random_walls            = false;
bool         Globals::Game::teleport                = false;
std::string  Globals::Game::current_level           = "";

ColorPair Globals::Theme::player_head;
ColorPair Globals::Theme::player_head_dead;
ColorPair Globals::Theme::player_body;
ColorPair Globals::Theme::fruit;

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

int Globals::Game::board_scroll_delay = 1000;

bool Globals::Game::board_scroll_up    = false;
bool Globals::Game::board_scroll_down  = false;
bool Globals::Game::board_scroll_left  = false;
bool Globals::Game::board_scroll_right = false;

bool Globals::Error::has_config_file = true;
bool Globals::Error::has_score_file  = true;
bool Globals::Error::old_version_score_file = false;
bool Globals::Error::strange_score_file     = false;

//  _   _      _  _____
// | | | |\ | | |  | |
// |_| |_| \| |_|  |_|

void Globals::init()
{
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
	                         "settings.ini");

	Globals::Config::scoresFile = (Globals::Config::directory +
	                               "arcade.nsnakescores");

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


	// Aww yeah, rev up dem colors
	Globals::Theme::player_head = Colors::pair("green", "default", true);
	Globals::Theme::player_head_dead = Colors::pair("red", "default", true);
	Globals::Theme::player_body = Colors::pair("green", "default", true);

	Globals::Theme::fruit = Colors::pair("red", "default", true);

	/// HACK Initializing the default level file directory.
	///      I know this is hacky, but couldn't find another way to
	///      initialize it.
	///
	BoardParser::directory = Globals::Config::directory + "levels/";
	ScoreFile::directory   = BoardParser::directory;

	/// Making sure they both exist...!
	if (! Utils::File::isDirectory(BoardParser::directory))
		Utils::File::mkdir_p(BoardParser::directory);
}
void Globals::loadFile()
{
	// Now, back on track
	if (! Utils::File::exists(Globals::Config::file))
		return;

	INI::Parser* ini = NULL;

	try {
		ini = new INI::Parser(Globals::Config::file);
	}
	catch(std::runtime_error& e)
	{
		// File doesn't exist (or we couldn't access it)
		// Either way, ignore it silently
		SAFE_DELETE(ini);
		return;
	}

	// Will be used on this macro below
	std::string buffer = "";

// Small macro to avoid unnecessary typing.
//
// To get something from the ini file we send the
// text (to identify some value) and the default
// value in case it doesn't exist.
//
// For the last one I send the variable itself,
// so we fallback to the default values.
#define INI_GET(var, out, in)                    \
	{                                            \
		buffer = (* ini)(out)[in];               \
		if (! buffer.empty())                    \
		{                                        \
			Utils::String::convert(buffer, var); \
		}                                        \
	}

	INI_GET(EngineGlobals::Screen::center_horizontally, "screen", "center_horizontal");
	INI_GET(EngineGlobals::Screen::center_vertically,   "screen", "center_vertical");

	INI_GET(EngineGlobals::Screen::show_borders,  "screen", "borders");
	INI_GET(EngineGlobals::Screen::fancy_borders, "screen", "fancy_borders");
	INI_GET(EngineGlobals::Screen::outer_border,  "screen", "outer_border");

	INI_GET(Globals::Game::random_walls,       "game", "random_walls");
	INI_GET(Globals::Game::fruits_at_once,     "game", "fruits_at_once");
	INI_GET(Globals::Game::teleport,           "game", "teleport");
	INI_GET(Globals::Game::board_scroll_delay, "game", "board_scroll_delay");

	INI_GET(Globals::Game::board_scroll_up,    "game", "board_scroll_up");
	INI_GET(Globals::Game::board_scroll_down,  "game", "board_scroll_down");
	INI_GET(Globals::Game::board_scroll_left,  "game", "board_scroll_left");
	INI_GET(Globals::Game::board_scroll_right, "game", "board_scroll_right");

	// unsigned ints are the exception - their overloading
	// is ambiguous... I should consider removing them altogether
	buffer = (* ini)("game")["starting_speed"];
	if (! buffer.empty())
	{
		int starting_speed = Globals::Game::starting_speed;
		Utils::String::convert(buffer, starting_speed);
		Globals::Game::starting_speed = starting_speed;
	}

	// Special Cases

	// Getting input keys
	std::string tmp;

	INI_GET(tmp, "input", "left");
	InputManager::bind("left", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "right");
	InputManager::bind("right", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "up");
	InputManager::bind("up", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "down");
	InputManager::bind("down", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "pause");
	InputManager::bind("pause", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "help");
	InputManager::bind("help", InputManager::stringToKey(tmp));

	INI_GET(tmp, "input", "quit");
	InputManager::bind("quit", InputManager::stringToKey(tmp));

	// Board Size
	int board_size = 2;
	INI_GET(board_size, "game", "board_size");
	Globals::Game::board_size = Globals::Game::intToBoardSize(board_size);

	// Getting the colors from their strings
	INI_GET(tmp, "gui_colors", "text");
	EngineGlobals::Theme::text = ColorPair::fromString(tmp);

	INI_GET(tmp, "gui_colors", "hilite_text");
	EngineGlobals::Theme::hilite_text = ColorPair::fromString(tmp);

	INI_GET(tmp, "gui_colors", "textbox");
	EngineGlobals::Theme::textbox = ColorPair::fromString(tmp);

	INI_GET(tmp, "game_colors", "player_head");
	Globals::Theme::player_head = ColorPair::fromString(tmp);

	INI_GET(tmp, "game_colors", "player_head_dead");
	Globals::Theme::player_head_dead = ColorPair::fromString(tmp);

	INI_GET(tmp, "game_colors", "player_body");
	Globals::Theme::player_body = ColorPair::fromString(tmp);

	INI_GET(tmp, "game_colors", "fruit");
	Globals::Theme::fruit = ColorPair::fromString(tmp);

	SAFE_DELETE(ini);
}
void Globals::saveFile()
{
	// Even if the file doesn't exist, we'll create it.
	INI::Parser* ini;

	try
	{
		ini = new INI::Parser(Globals::Config::file);
	}
	catch(std::runtime_error& e)
	{
		ini = new INI::Parser();
		ini->create();
	}

	// Will be used on this macro below
	std::string buffer;

// Other macro to avoid typing, similar to the one
// at loadFile()
#define INI_SET(out, in, var)	               \
	{                                          \
		buffer = Utils::String::toString(var); \
		ini->top().addGroup(out);              \
		(* ini)(out).addKey(in, buffer);       \
	}

	INI_SET("screen", "center_horizontal", EngineGlobals::Screen::center_horizontally);
	INI_SET("screen", "center_vertical",   EngineGlobals::Screen::center_vertically);

	INI_SET("screen", "borders",       EngineGlobals::Screen::show_borders);
	INI_SET("screen", "fancy_borders", EngineGlobals::Screen::fancy_borders);
	INI_SET("screen", "outer_border",  EngineGlobals::Screen::outer_border);

	INI_SET("game", "random_walls",     Globals::Game::random_walls);
	INI_SET("game", "fruits_at_once",   Globals::Game::fruits_at_once);
	INI_SET("game", "teleport",         Globals::Game::teleport);

	INI_SET("game", "board_scroll_delay", Globals::Game::board_scroll_delay);

	INI_SET("game", "board_scroll_up",    Globals::Game::board_scroll_up);
	INI_SET("game", "board_scroll_down",  Globals::Game::board_scroll_down);
	INI_SET("game", "board_scroll_left",  Globals::Game::board_scroll_left);
	INI_SET("game", "board_scroll_right", Globals::Game::board_scroll_right);

	// unsigned ints are the exception - their overloading
	// is ambiguous... I should consider removing them altogether
	int starting_speed = Globals::Game::starting_speed;
	buffer = Utils::String::toString(starting_speed);
	ini->top().addGroup("game");
	(* ini)("game").addKey("starting_speed", buffer);

	// Special Cases

	// Input Keys
	std::string key;

	key = InputManager::keyToString(InputManager::getBind("left"));
	INI_SET("input", "left", key);

	key = InputManager::keyToString(InputManager::getBind("right"));
	INI_SET("input", "right", key);

	key = InputManager::keyToString(InputManager::getBind("up"));
	INI_SET("input", "up", key);

	key = InputManager::keyToString(InputManager::getBind("down"));
	INI_SET("input", "down", key);

	key = InputManager::keyToString(InputManager::getBind("pause"));
	INI_SET("input", "pause", key);

	key = InputManager::keyToString(InputManager::getBind("help"));
	INI_SET("input", "help", key);

	key = InputManager::keyToString(InputManager::getBind("quit"));
	INI_SET("input", "quit", key);

	// Board size
	int board_size = Globals::Game::boardSizeToInt(Globals::Game::board_size);
	INI_SET("game", "board_size", board_size);

	// Saving the colors from their strings
	INI_SET("gui_colors", "text", EngineGlobals::Theme::text.toString());

	INI_SET("gui_colors", "hilite_text", EngineGlobals::Theme::hilite_text.toString());

	INI_SET("gui_colors", "textbox", EngineGlobals::Theme::textbox.toString());


	INI_SET("game_colors", "player_head", Globals::Theme::player_head.toString());

	INI_SET("game_colors", "player_head_dead", Globals::Theme::player_head_dead.toString());

	INI_SET("game_colors", "player_body", Globals::Theme::player_body.toString());

	INI_SET("game_colors", "fruit", Globals::Theme::fruit.toString());

	try
	{
		ini->saveAs(Globals::Config::file);
	}
	catch(std::runtime_error& e)
	{
		// Couldn't save the file...
		// ... do nothing
	}
	SAFE_DELETE(ini);
}
void Globals::warnErrors()
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

