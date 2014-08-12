#ifndef SCORE_H_DEFINED
#define SCORE_H_DEFINED

#include <Config/Globals.hpp>

#include <string>
#include <vector>
#include <exception>

/// Custom exception class to specify an error that
/// occurred during a level loading.
///
class ScoreFileException : public std::exception
{
public:
	ScoreFileException(std::string message):
		message(message)
	{ }
	~ScoreFileException() throw()
	{ }

	std::string message;
};

/// A single entry on the high-score file.
///
struct ScoreEntry
{
	/// How many points the user got.
	unsigned int points;

	/// Under which game speed the score was made.
	unsigned int speed;

	/// On which level the user made this score.
	/// It defaults to "", which is the Arcade Mode.
	std::string level;

	/// How many fruits at once were allowed on this level.
	int fruits;

	/// If random walls were spawned on this level.
	bool random_walls;

	/// If teleport was enabled on this level.
	bool teleport;

	/// How large was the game board on this score.
	///
	/// @note This is only valid on the Arcade Mode.
	///       Levels don't care about it.
	Globals::Game::BoardSize board_size;

	int  board_scroll_delay;
	bool board_scroll_left;
	bool board_scroll_right;
	bool board_scroll_up;
	bool board_scroll_down;

	/// Creates an empty score entry.
	/// Since everything is public, access them
	/// without thinking twice.
	ScoreEntry();

	/// Tells if both scores were made on exact same game settings.
	///
	/// Scores are incompatible if they doesn't have the same
	/// game settings.
	/// It's unfair for a score with less fruits to be compared
	/// with one where more fruits were enabled.
	///
	bool isLike(ScoreEntry& other);
};

/// Stores points the player made on the game.
///
/// There's different high scores for different
/// game settings.
///
/// Each time the user runs the game with a different
/// configuration of the tweaks above, a new high
/// score is generated.
///
/// It always starts with 0 and if the player
/// surpasses it, will be the new maximum.
///
class ScoreFile
{
public:
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

	/// Erases all high score files.
	///
	/// By "all" I mean every single score file from the arcade
	/// mode to all levels.
	///
	static void eraseAll();

	/// Creates a new score handler for the level #levelName.
	///
	/// @note It doesn't actually work unless you call #load, mate
	ScoreFile(std::string levelName);

	/// Loads all high score entries based on a level name.
	///
	/// The file where we save the high scores will be based
	/// on the level name.
	///
	/// @note If `levelName` is empty, will fall back to
	///       the default score file (see Globals).
	///
	void load();

	/// Saves all the current scores on the file.
	void save();

	/// Checks if #score is the highest score and
	/// make it so.
	///
	/// Call this when you finished the game.
	///
	/// @return If #score just became the highest score.
	bool handle(ScoreEntry* score);

	/// Maximum high score obtained for the current game.
	///
	/// It will point to an element within #entries that
	/// has the highest score for this game settings.
	///
	/// @note If this is NULL then we couldn't open the
	///       high score files!
	///
	ScoreEntry* highScore;

private:
	/// Name of the level under these scores fall into.
	/// Defaults to "" for Arcade Mode.
	std::string level_name;

	/// All score entries for this file
	std::vector<ScoreEntry>	entries;
};

#endif //SCORE_H_DEFINED

