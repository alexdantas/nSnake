#ifndef SCORE_H_DEFINED
#define SCORE_H_DEFINED

#include <string>

/// Stores points the player made on the game.
struct Score
{
	unsigned int points;
	unsigned int level;

	Score();

	/// Loads the high score for the current game from global
	/// file.
	static void loadFile();

	/// Saves the high score for the current game on global
	/// file.
	static void saveFile();
};

#endif //SCORE_H_DEFINED

