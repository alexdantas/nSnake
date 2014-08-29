#include <Entities/ScoreFile.hpp>
#include <Entities/BoardParser.hpp>
#include <Engine/Helpers/File.hpp>
#include <Engine/Helpers/String.hpp>
#include <Engine/Helpers/INI.hpp>
#include <Engine/Helpers/Base64.hpp>

#include <stdlib.h>	  // getenv()
#include <fstream>    // ofstream

// HACK This will be initialized at `Globals::init()`
std::string ScoreFile::directory = "";

std::string ScoreFile::extension = "nsnakescores";


ScoreEntry::ScoreEntry():
	points(0),
	speed(0),
	level(""),
	fruits(0),
	random_walls(false),
	teleport(false),
	board_size(Globals::Game::LARGE),
	board_scroll_delay(0),
	board_scroll_left(false),
	board_scroll_right(false),
	board_scroll_up(false),
	board_scroll_down(false)
{ }

bool ScoreEntry::isLike(ScoreEntry& other)
{
	if (this->level != other.level)
		return false;

	// First thing we gotta check is if this score
	// was made on Arcade Mode (level is empty)
	//
	// If that's the case, then we care for the
	// board size
	if (this->level.empty())
	{
		return (this->fruits       == other.fruits       &&
		        this->random_walls == other.random_walls &&
		        this->teleport     == other.teleport     &&
		        this->speed        == other.speed        &&
		        this->board_scroll_delay == other.board_scroll_delay &&
		        this->board_scroll_left  == other.board_scroll_left  &&
		        this->board_scroll_right == other.board_scroll_right &&
		        this->board_scroll_up    == other.board_scroll_up    &&
		        this->board_scroll_down  == other.board_scroll_down  &&
		        this->board_size   == other.board_size);
	}

	// If not, the board size is not important, since levels
	// can have any size.
	return (this->fruits       == other.fruits       &&
	        this->random_walls == other.random_walls &&
	        this->teleport     == other.teleport     &&
	        this->speed        == other.speed        &&
	        this->board_scroll_delay == other.board_scroll_delay &&
	        this->board_scroll_left  == other.board_scroll_left  &&
	        this->board_scroll_right == other.board_scroll_right &&
	        this->board_scroll_up    == other.board_scroll_up    &&
	        this->board_scroll_down  == other.board_scroll_down);
}





void ScoreFile::eraseAll()
{
	// 1. Delete the arcade score fileerase this one
	// 2. Lists all files under the score dir and erase
	//    the ones ending with a score extension

	Utils::File::rm_f(Globals::Config::scoresFile);

	std::vector<std::string> files = Utils::File::ls(ScoreFile::directory);

	for (size_t i = 0; i < files.size(); i++)

		if (Utils::File::extension(files[i]) == ScoreFile::extension)
			Utils::File::rm_f(files[i]);
}





ScoreFile::ScoreFile(std::string levelName):
	highScore(NULL),
	level_name(levelName)
{ }

void ScoreFile::load()
{
	// Make it point nowhere, since we're refreshing
	// the score entries.
	this->highScore = NULL;

	// Score files are dependent of the level name.
	std::string score_file = (ScoreFile::directory +
	                          this->level_name +
	                          "." +
	                          ScoreFile::extension);

	// Will fall back to default high score file
	// (Arcade Mode) if no level was specified
	if (this->level_name.empty())
		score_file = Globals::Config::scoresFile;

	if (! Utils::File::exists(score_file))
		throw ScoreFileException("File '" + score_file + "' doesn't exist");

	// Reading whole file's contents into a buffer
	std::ifstream file;
	file.open(score_file.c_str());

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	std::stringstream contents;
	contents << Utils::Base64::decode(buffer.str());

	// Parsing file's contents as INI
	INI::Parser ini(contents);

	// If it's a score file from a different major version,
	// how should we react?
	// No need to worry about minor versions.
	std::string version = ini["version"];

	if (version[0] != Globals::version[MAJOR])
	{
		// Compare versions, lower, higher, whatever...
		Globals::Error::old_version_score_file = true;

		throw ScoreFileException("File '" + score_file + "' has an old version format");
	}

	// Going through each group on the INI file
	// (each score the user had)
	for (INI::Level::Sections::const_iterator it = ini.top().ordered_sections.begin();
	     it != ini.top().ordered_sections.end();
	     ++it)
	{
		// This is SOO ugly!
		// We should NOT have to worry about INI parser's internals!
		INI::Level ini_score = (*it)->second;

		ScoreEntry entry;
		entry.level  = ini_score["level"];
		entry.points = Utils::String::to<unsigned int>(ini_score["points"]);
		entry.speed  = Utils::String::to<unsigned int>(ini_score["speed"]);
		entry.fruits = Utils::String::to<int>(ini_score["fruits"]);
		entry.random_walls = Utils::String::to<bool>(ini_score["random_walls"]);
		entry.teleport     = Utils::String::to<bool>(ini_score["teleport"]);

		entry.board_scroll_delay = Utils::String::to<int>(ini_score["board_scroll_delay"]);
		entry.board_scroll_left  = Utils::String::to<bool>(ini_score["board_scroll_left"]);
		entry.board_scroll_right = Utils::String::to<bool>(ini_score["board_scroll_right"]);
		entry.board_scroll_up    = Utils::String::to<bool>(ini_score["board_scroll_up"]);
		entry.board_scroll_down  = Utils::String::to<bool>(ini_score["board_scroll_down"]);

		int board_size = Utils::String::to<int>(ini_score["board_size"]);
		entry.board_size = Globals::Game::intToBoardSize(board_size);

		this->entries.push_back(entry);
	}

	// Finally, we have to pick the highest score
	// according to these game settings.
	ScoreEntry tmp_score;
	tmp_score.level        = this->level_name;
	tmp_score.speed        = Globals::Game::starting_speed;
	tmp_score.fruits       = Globals::Game::fruits_at_once;
	tmp_score.random_walls = Globals::Game::random_walls;
	tmp_score.teleport     = Globals::Game::teleport;
	tmp_score.board_size   = Globals::Game::board_size;
	tmp_score.board_scroll_delay = Globals::Game::board_scroll_delay;
	tmp_score.board_scroll_left  = Globals::Game::board_scroll_left;
	tmp_score.board_scroll_right = Globals::Game::board_scroll_right;
	tmp_score.board_scroll_up    = Globals::Game::board_scroll_up;
	tmp_score.board_scroll_down  = Globals::Game::board_scroll_down;

	for (size_t i = 0; i < (this->entries.size()); i++)
	{
		if (tmp_score.isLike(this->entries[i]))
		{
			this->highScore = &(this->entries[i]);
			break;
		}
	}
	if (this->highScore == NULL)
	{
		this->entries.push_back(tmp_score);
		this->highScore = &(this->entries[this->entries.size() - 1]);
	}
}
void ScoreFile::save()
{
	// Score files are dependent of the level name.
	std::string score_file = (ScoreFile::directory +
	                          this->level_name +
	                          "." +
	                          ScoreFile::extension);

	// Will fall back to default high score file
	// if no level was specified
	if (this->level_name.empty())
		score_file = Globals::Config::scoresFile;

	// Tries to create file if it doesn't exist.
	// If we can't create it at all let's just give up.
	if (! Utils::File::exists(score_file))
	{
		Utils::File::create(score_file);

		if (! Utils::File::exists(score_file))
			throw ScoreFileException("Could not create file '" + score_file + "'");
	}

	// We'll recreate the whole score file from scratch
	INI::Parser ini;
	ini.create();
	ini.top().addKey("version", std::string(VERSION));

	// Adding each score entry on the file
	for (size_t i = 0; i < (this->entries.size()); i++)
	{
		std::string score_name = "score" + Utils::String::toString(i);

		ini.top().addGroup(score_name);

		ini(score_name).addKey("level", this->entries[i].level);
		ini(score_name).addKey("points", Utils::String::toString(this->entries[i].points));
		ini(score_name).addKey("speed",  Utils::String::toString(this->entries[i].speed));
		ini(score_name).addKey("fruits", Utils::String::toString(this->entries[i].fruits));

		ini(score_name).addKey("random_walls", Utils::String::toString(this->entries[i].random_walls));
		ini(score_name).addKey("teleport", Utils::String::toString(this->entries[i].teleport));

		int board_size = Globals::Game::boardSizeToInt(this->entries[i].board_size);
		ini(score_name).addKey("board_size", Utils::String::toString(board_size));

		ini(score_name).addKey("board_scroll_delay", Utils::String::toString(this->entries[i].board_scroll_delay));
		ini(score_name).addKey("board_scroll_left", Utils::String::toString(this->entries[i].board_scroll_left));
		ini(score_name).addKey("board_scroll_right", Utils::String::toString(this->entries[i].board_scroll_right));
		ini(score_name).addKey("board_scroll_up", Utils::String::toString(this->entries[i].board_scroll_up));
		ini(score_name).addKey("board_scroll_down", Utils::String::toString(this->entries[i].board_scroll_down));
	}

	std::stringstream contents;
	ini.dump(contents);

	std::ofstream file;
	file.open(score_file.c_str());
	file << Utils::Base64::encode(contents.str());
	file.close();
}
bool ScoreFile::handle(ScoreEntry* score)
{
	// No high score until now, we've made it!
	if (! this->highScore)
	{
		this->entries.push_back(*score);
		this->highScore = &(this->entries[this->entries.size() - 1]);
		return true;
	}

	// Wrong game settings?
	if (! score->isLike(*this->highScore))
		return false;

	if ((score->points) > (this->highScore->points))
	{
		this->highScore->points = score->points;

		return true;
	}
	return false;
}

