#ifndef SCORE_H_DEFINED
#define SCORE_H_DEFINED

#include <string>

/// Stores points the player made on the game.
struct Score
{
	/// Default directory where we store the game score files.
	///
	/// By default is the same directory as the level files
	/// (see BoardParser).
	///
	/// So the default behavior is to create a score file with
	/// the same name as the level and a custom extension.
	static std::string directory;

	/// Default extension to save the score files.
	///
	/// It's all the part that comes _after the dot_ on
	/// a file name.
	///
	/// @note Defaults to "nsnakescores"
	static std::string extension;

	unsigned int points;
	unsigned int speed;

	/// Creates a new score instance based on a level name.
	///
	/// The file where we save the high scores will be based
	/// on the level name.
	///
	/// @note If `levelName` is empty, will fall back to
	///       the default score file (see Globals).
	///
	Score(std::string levelName="");

	/// Loads the high score for the current game level.
	void loadFile();

	/// Saves the high score of the current game level.
	void saveFile();

	std::string level;
};

#endif //SCORE_H_DEFINED

