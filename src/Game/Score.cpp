#include <Game/Score.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>
#include <Game/BoardParser.hpp>

#include <stdlib.h>	  // getenv()
#include <fstream>    // ofstream

// HACK This will be initialized at `Globals::init()`
std::string Score::directory = "";

std::string Score::extension = "nsnakescores";

Score::Score(std::string levelName):
	points(0),
	speed(0),
	level(levelName)
{ }

void Score::loadFile()
{
	// Default high score for any mode.
	Globals::Game::highScore.points = 0;

	// Score files are dependent of the level name.
	std::string score_file = (Score::directory +
	                          this->level +
	                          "." +
	                          Score::extension);

	// Will fall back to default high score file
	// if no level was specified
	if (this->level.empty())
		score_file = Globals::Config::scoresFile;

	if (! Utils::File::exists(score_file))
	    return;

	std::fstream file;
	file.open(score_file.c_str(), std::ios::in | std::ios::binary);

	// Let's see if the file's empty.
	int major = Globals::version[MAJOR];
	file.read((char*) &major, sizeof(major));

	// If that's the case, #saveFile() will fill it
	// with the default values.
	if (file.eof())
		return;

// Macro that reads on #a an amout of #sizeof(a).
// Also, if there's any reading error it aborts execution
// right away, setting the global flag.
#define READ(a)	  \
	{ \
		file.read((char*) &a, sizeof(a)); \
		if (!file) \
		{ \
			break; \
		} \
	} \

	if (major != Globals::version[MAJOR])
	{
		// Woops! Incompatibility issues!
		// We can't read the high score file
		Globals::Error::old_version_score_file = true;

		// It is desirable that we convert the old format
		// to the new one - for now we're going to simply
		// ignore it.
		return;
	}

	// Now we'll scan all the saved scores
	// for one that matches the current game.
	bool found = false;
	do
	{
		int   fruits;
		bool  random_walls;
		bool  teleport;
		int   board_size;
		Score score;

		READ(fruits);
		READ(random_walls);
		READ(teleport);
		READ(board_size);

		// Reading a high score
		READ(score.speed);
		READ(score.points);

		// Will only care about the score if it's settings
		// are exactly as the current Game's.
		if (fruits         == Globals::Game::fruits_at_once &&
		    random_walls   == Globals::Game::random_walls   &&
		    teleport       == Globals::Game::teleport       &&
		    score.speed    == Globals::Game::starting_speed)
		{
			// Will only care for the board size if we're at
			// the default level. It doesn't matter on other levels
			if (this->level.empty())
			{
				if (board_size == Globals::Game::boardSizeToInt(Globals::Game::board_size))
				{
					found = true;

					Globals::Game::highScore.speed  = score.speed;
					Globals::Game::highScore.points = score.points;
				}
			}
			else
			{
				found = true;

				Globals::Game::highScore.speed  = score.speed;
				Globals::Game::highScore.points = score.points;
			}
		}

	} while(!file.eof() && !found);

	// If we couldn't find the score, it's already set
	// as a default value anyways.
}
void Score::saveFile()
{
	// Score files are dependent of the level name.
	std::string score_file = (Score::directory +
	                          this->level +
	                          "." +
	                          Score::extension);

	// Will fall back to default high score file
	// if no level was specified
	if (this->level.empty())
		score_file = Globals::Config::scoresFile;

	// Tries to create file if it doesn't exist.
	// If we can't create it at all let's just give up.
	if (! Utils::File::exists(score_file))
	{
		Utils::File::create(score_file);

		if (! Utils::File::exists(score_file))
			return;
	}

	// Finally start doing things to an actual file!

	std::fstream file;
	file.open(score_file.c_str(),
	          std::ios::in | std::ios::out | std::ios::binary);

// Macro that writes #a to file, same as the READ
// macro above.
#define WRITE(a) \
	{ \
		file.write((char*) &a, sizeof(a)); \
		if (!file) \
		{ \
			return; \
		} \
	}

	// The only toleance we have is for empty files.
	// If that's the case, we'll write everything up.
	int version;
	file.read((char*) &version, sizeof(version));

	if (file.eof())
	{
		// Writing everything on the file and ending it.
		file.clear();

		// Making sure the first thing on the file is the
		// current version's Major number.
		WRITE(Globals::version[MAJOR]);
		WRITE(Globals::Game::fruits_at_once);
		WRITE(Globals::Game::random_walls);
		WRITE(Globals::Game::teleport);


		if (this->level.empty())
		{
			// Clever hack: The option "board_size" is only valid for
			//              the default level.
			//              It should not make any difference on levels
			//              scores.
			//              So we'll default it to "Large" when writing
			//              to the file only if not at default level.
			int size = Globals::Game::boardSizeToInt(Globals::Game::board_size);

			WRITE(size);
		}
		else
		{
			int size = Globals::Game::boardSizeToInt(Globals::Game::LARGE);

			WRITE(size);
		}

		// Writing the high score
		WRITE(Globals::Game::highScore.speed);
		WRITE(Globals::Game::highScore.points);
		return;
	}

	// If we got here, the file's not empty.
	// So we'll not tolerate reading errors!

	// Now we'll scan all the saved scores
	// for one that matches the current game.
	bool found = false;
	do
	{
		int fruits;
		bool random_walls;
		bool teleport;
		int board_size;
		Score score;

		READ(fruits);
		READ(random_walls);
		READ(teleport);
		READ(board_size);
		READ(score.speed);

		// Will only care about the score if it's settings
		// are exactly as the current Game's.
		if (fruits         == Globals::Game::fruits_at_once &&
		    random_walls   == Globals::Game::random_walls   &&
		    teleport       == Globals::Game::teleport       &&
		    score.speed    == Globals::Game::highScore.speed)
		{
			// Will only care for the board size if we're at
			// the default level. It doesn't matter on other levels
			if (this->level.empty())
			{
				if (board_size == Globals::Game::boardSizeToInt(Globals::Game::board_size))
				{
					found = true;

					WRITE(Globals::Game::highScore.points);
				}
			}
			else
			{
				found = true;

				WRITE(Globals::Game::highScore.points);
			}
		}
		else
		{
			// Positioning the cursor right away
			Score score;

			READ(score.points);
		}

	} while(!file.eof() && !found);

	// If we couldn't find the score, let's append it!
	if (!found)
	{
		if (file.eof())
			file.clear();

		WRITE(Globals::Game::fruits_at_once);
		WRITE(Globals::Game::random_walls);
		WRITE(Globals::Game::teleport);

		if (this->level.empty())
		{
			// Will only care for the board size if we're at
			// the default level. It doesn't matter on other levels
			int size = Globals::Game::boardSizeToInt(Globals::Game::board_size);

			WRITE(size);
		}
		else
		{
			int size = Globals::Game::boardSizeToInt(Globals::Game::LARGE);
			WRITE(size);
		}

		WRITE(Globals::Game::highScore.speed);
		WRITE(Globals::Game::highScore.points);
	}
}

