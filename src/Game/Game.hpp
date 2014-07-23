#ifndef GAME_H_DEFINED
#define GAME_H_DEFINED

#include <Game/Score.hpp>
#include <Game/Player.hpp>
#include <Game/Board.hpp>
#include <Game/FruitManager.hpp>
#include <Misc/Timer.hpp>
#include <Interface/Menu/Menu.hpp>

#include <vector>

/// Pre-defining it's layout to avoid circular dependency.
class LayoutGame;

class Game
{
	friend class LayoutGame;

public:
	Game();
	virtual ~Game();

	/// Starts game, optionally loading a level.
	///
	/// @note This is not the path to the level file!
	///       It's merely a level name.
	///
	/// @see BoardParser::load
	///
	/// @note If no level name is specified, will default
	///       to an empty box.
	///
	void start(std::string levelName="");

	void handleInput();
	void update();
	void draw();

	bool isOver();

	/// If we'll quit the game right away.
	bool willQuit();

	/// If we'll return to the main menu.
	bool willReturnToMenu();

	/// Returns how much time (millisseconds) we need to wait
	/// for a specific #speed.
	int getDelay(int speed);

	void pause(bool option);

	// GameStateGame needs them to be public
	Score* score;
	Score* highScore;

protected:
	LayoutGame* layout;

	/// If the game is over (board is full of blocks).
	bool gameOver;

	bool userAskedToQuit;
	bool userAskedToGoToMenu;

	/// Timer that tells when to move the player, based
	/// on the current speed).
	Timer timerSnake;

	// Times how long the user have been playing the game.
	Timer timer;

	/// If the game is paused.
	/// May show other Windows while paused.
	bool isPaused;

	/// If it's showing the pause menu.
	/// Goes together with #isPaused.
	bool showPauseMenu;

	/// If it's showing the help screen.
	/// Goes together with #isPaused.
	bool showHelp;

	/// Menu that's shown only when the user presses Pause.
	Menu* pauseMenu;

	Player* player;
	Board* board;
	FruitManager* fruits;
};

#endif //GAME_H_DEFINED

