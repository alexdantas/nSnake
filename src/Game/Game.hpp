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

	void start();
	void handleInput();
	void update();
	void draw();

	bool isOver();

	/// If we'll quit the game right away.
	bool willQuit();

	/// If we'll return to the main menu.
	bool willReturnToMenu();

	/// Returns how much time (millisseconds) we need to wait
	/// for a specific #level.
	int getDelay(int level);

	void pause(bool option);

protected:
	LayoutGame* layout;

	/// If the game is over (board is full of blocks).
	bool gameOver;

	bool userAskedToQuit;
	bool userAskedToGoToMenu;

	/// Timer that tells when to move the player, based
	/// on the current level).
	Timer timerSnake;

	// Times how long the user have been playing the game.
	Timer timer;

	Score* score;
	Score* highScore;

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

