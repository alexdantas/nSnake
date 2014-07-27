#include <Game/Game.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>
#include <Interface/LayoutGame.hpp>
#include <Flow/InputManager.hpp>
#include <Game/BoardParser.hpp>

#include <stdlib.h>

// Options of the Pause Menu
enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	RESUME, RESTART, QUIT_MENU, QUIT_GAME
};

Game::Game():
	scores(NULL),
	currentScore(NULL),
	layout(NULL),
	gameOver(false),
	isPaused(false),
	showPauseMenu(false),
	showHelp(false),
	pauseMenu(NULL),
	player(NULL),
	board(NULL),
	fruits(NULL)
{ }
Game::~Game()
{
	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->scores);
	SAFE_DELETE(this->currentScore);
	SAFE_DELETE(this->pauseMenu);
	SAFE_DELETE(this->player);
	SAFE_DELETE(this->board);
	SAFE_DELETE(this->fruits);
}
void Game::start(std::string levelName)
{
	// Cleaning things from the previous game (if any)
	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->scores);
	SAFE_DELETE(this->currentScore);
	SAFE_DELETE(this->pauseMenu);
	SAFE_DELETE(this->player);
	SAFE_DELETE(this->board);
	SAFE_DELETE(this->fruits);

	this->userAskedToQuit     = false;
	this->userAskedToGoToMenu = false;
	this->gameOver            = false;
	this->isPaused            = false;

	// The interface
	this->layout = new LayoutGame(this, 80, 24);

	this->scores = new ScoreFile(levelName);
	// will load the scores on `GameStateGame`

	this->currentScore = new ScoreEntry();
	this->currentScore->level        = levelName;
	this->currentScore->speed        = Globals::Game::starting_speed;
	this->currentScore->fruits       = Globals::Game::fruits_at_once;
	this->currentScore->random_walls = Globals::Game::random_walls;
	this->currentScore->teleport     = Globals::Game::teleport;
	this->currentScore->board_size   = Globals::Game::board_size;

	// Creating the menu and adding each item
	this->pauseMenu = new Menu(1,
	                           1,
	                           this->layout->pause->getW() - 2,
	                           this->layout->pause->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Resume", RESUME);
	this->pauseMenu->add(item);

	item = new MenuItem("Restart", RESTART);
	this->pauseMenu->add(item);

	this->pauseMenu->addBlank();

	item = new MenuItem("Quit to Main Menu", QUIT_MENU);
	this->pauseMenu->add(item);

	item = new MenuItem("Quit Game", QUIT_GAME);
	this->pauseMenu->add(item);

	// Defaults to large
	int boardw = 78;
	int boardh = 21;

	if (Globals::Game::board_size == Globals::Game::SMALL)
	{
		boardw = 40;
		boardh = 10;
	}
	else if (Globals::Game::board_size == Globals::Game::MEDIUM)
	{
		boardw = 55;
		boardh = 14;
	}

	if (! levelName.empty())
		this->board = BoardParser::load(levelName);

	else
	{
		// If no level name is specified, silently
		// fall back to a default one.

		this->board = new Board(boardw,
		                        boardh,
		                        ((Globals::Game::teleport) ?
		                         Board::TELEPORT :
		                         Board::SOLID));
	}

	// the player!
	this->player = new Player(this->board->getStartX(),
	                          this->board->getStartY());

	if (Globals::Game::random_walls)
		this->board->randomlyFillExceptBy(this->player->getX(),
		                                  this->player->getY());

	// fruits beibeh
	this->fruits = new FruitManager(Globals::Game::fruits_at_once);
	this->fruits->update(this->player, this->board);

	// Starting timers
	this->timerSnake.start();
	this->timer.start();
}
void Game::handleInput()
{
	if (InputManager::noKeyPressed())
		return;

	// The only two absolute inputs are to quit and pause.
	// Others depend if the game is paused or not.
	if (InputManager::isPressed("quit"))
	{
		this->userAskedToQuit = true;
	}
	else if (InputManager::isPressed("pause"))
	{
		(this->isPaused) ?
			this->pause(false) :
			this->pause(true);

		return;
	}
	else if (InputManager::isPressed('\n') ||
	         InputManager::isPressed(KEY_ENTER))
	{
		if (! this->isPaused)
		{
			this->pause(true);
			return;
			// This needs to be here otherwise
			// ENTER goes to the menu and immediately
			// unpauses the game.
		}
	}
	else if (InputManager::isPressed("help"))
	{
		// Toggling Pause and Help window
		if (this->isPaused)
		{
			this->showHelp = false;
			this->timer.unpause();
			this->timerSnake.unpause();
		}
		else
		{
			this->showHelp = true;
			this->timer.pause();
			this->timerSnake.pause();
		}
	}

	// Other keys are not used when paused.
	if (this->isPaused || this->showHelp)
	{
		this->pauseMenu->handleInput();
		return;
	}

	if (InputManager::isPressed("left"))
	{
		this->player->move(Player::LEFT);
	}
	else if (InputManager::isPressed("right"))
	{
		this->player->move(Player::RIGHT);
	}
	else if (InputManager::isPressed("up"))
	{
		this->player->move(Player::UP);
	}
	else if (InputManager::isPressed("down"))
	{
		this->player->move(Player::DOWN);
	}
}
void Game::update()
{
	if (this->gameOver)
		return;

	// If we're paused, only handle the menu.
	if (this->isPaused)
	{
		if (this->pauseMenu->willQuit())
		{
			int option = this->pauseMenu->currentID();

			switch(option)
			{
			case RESUME:
				this->pause(false);
				break;

			case RESTART:
				this->start(Globals::Game::current_level);
				return;

			case QUIT_MENU:
				this->userAskedToGoToMenu = true;
				break;

			case QUIT_GAME:
				this->userAskedToQuit = true;
				break;
			}
			this->pauseMenu->reset();
		}
		return;
	}

	// Forcing Snake to move if enough time has passed
	// (time based on current level)
	this->timerSnake.pause();
	int delta = this->getDelay(this->currentScore->speed);

	if (this->timerSnake.delta_ms() >= delta)
	{
		// Checking if on the previous frame
		// the Snake died.
		if (! this->player->isAlive())
		{
			this->gameOver = true;

			// Check the return value and warns the player
			// if he just beat the high score
			this->scores->handle(this->currentScore);
		}
		else
		{
			// Actually move the player
			this->player->update(this->board);

			while (this->fruits->eatenFruit(this->player))
			{
				this->player->increase();

				// Score formula is kinda random and
				// scattered all over this file.
				// TODO: Center it all on the Score class.
				this->currentScore->points += this->currentScore->speed * 2;
			}

			this->fruits->update(this->player, this->board);
		}
		this->timerSnake.start();
	}
	else
		this->timerSnake.unpause();
}
void Game::draw()
{
	this->layout->draw(this->pauseMenu);
}
bool Game::isOver()
{
	return (this->gameOver);
}
bool Game::willQuit()
{
	return this->userAskedToQuit;
}
bool Game::willReturnToMenu()
{
	return this->userAskedToGoToMenu;
}
int Game::getDelay(int speed)
{
	// returning delay in milliseconds
	if (speed < 1) return 800;

	switch (speed)
	{
	case 1:  return 800;
	case 2:  return 600;
	case 3:  return 500;
	case 4:  return 300;
	case 5:  return 200;
	case 6:  return 150;
	case 7:  return 125;
	case 8:  return 100;
	case 9:  return 80;
	case 10: return 50;
	}
	return 50;
}
void Game::pause(bool option)
{
	if (option)
	{
		if (this->isPaused)
			return;

		this->isPaused = true;
		this->showPauseMenu = true;
		this->timer.pause();
		this->timerSnake.pause();
	}
	else
	{
		if (! this->isPaused)
			return;

		this->isPaused = false;
		this->showPauseMenu = false;
		this->timer.unpause();
		this->timerSnake.unpause();
	}
}

