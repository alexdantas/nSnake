#include <Game/Score.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

#include <stdlib.h>	  // getenv()
#include <fstream>    // ofstream

Score::Score():
	points(0),
	level(0)
{ }

void Score::loadFile()
{
	// Default high score for any mode.
	Globals::Game::highScore.points = 0;

	if (! Utils::File::exists(Globals::Config::scoresFile))
		return;

	std::fstream file;
	file.open(Globals::Config::scoresFile.c_str(), std::ios::in | std::ios::binary);

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
		int fruits;
		bool random_walls;
		bool teleport;
		int board_size;
		Score score;

		READ(fruits);
		READ(random_walls);
		READ(teleport);
		READ(board_size);

		// Reading a high score
		READ(score.level);
		READ(score.points);

		// Will only care about the score if it's settings
		// are exactly as the current Game's.
		if (fruits         == Globals::Game::fruits_at_once &&
		    random_walls   == Globals::Game::random_walls   &&
		    teleport       == Globals::Game::teleport       &&
		    board_size     == Globals::Game::boardSizeToInt(Globals::Game::board_size) &&
		    score.level    == Globals::Game::starting_level)
		{
			found = true;

			Globals::Game::highScore.level  = score.level;
			Globals::Game::highScore.points = score.points;
		}

	} while(!file.eof() && !found);

	// If we couldn't find the score, it's already set
	// as a default value anyways.
}
void Score::saveFile()
{
	// Tries to create file if it doesn't exist.
	// If we can't create it at all let's just give up.
	if (! Utils::File::exists(Globals::Config::scoresFile))
	{
		Utils::File::create(Globals::Config::scoresFile);

		if (! Utils::File::exists(Globals::Config::scoresFile))
			return;
	}

	std::fstream file;
	file.open(Globals::Config::scoresFile.c_str(), std::ios::in | std::ios::out | std::ios::binary);

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

		int size = Globals::Game::boardSizeToInt(Globals::Game::board_size);
		WRITE(size);

		// Writing the high score
		WRITE(Globals::Game::highScore.level);
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
		READ(score.level);

		// Will only care about the score if it's settings
		// are exactly as the current Game's.
		if (fruits         == Globals::Game::fruits_at_once &&
		    random_walls   == Globals::Game::random_walls   &&
		    teleport       == Globals::Game::teleport       &&
		    board_size     == Globals::Game::boardSizeToInt(Globals::Game::board_size) &&
		    score.level    == Globals::Game::highScore.level)
		{
			found = true;

			WRITE(Globals::Game::highScore.points);
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

		int size = Globals::Game::boardSizeToInt(Globals::Game::board_size);
		WRITE(size);

		WRITE(Globals::Game::highScore.level);
		WRITE(Globals::Game::highScore.points);
	}
}

