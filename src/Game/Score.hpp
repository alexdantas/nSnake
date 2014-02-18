#ifndef SCORE_H_DEFINED
#define SCORE_H_DEFINED

#include <string>

/// Stores points the player made on the game.
struct Score
{
	/// Name of the player.
	/// An interesting thing is that we try to get the user
	/// name first - then asking the user for a specific name.
	std::string name;

	unsigned int points;
	unsigned int lines;
	unsigned int level;

	Score();
};

#endif //SCORE_H_DEFINED

