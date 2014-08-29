#include <States/GameStateGame.hpp>
#include <Engine/Flow/StateManager.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/Graphics/Widgets/Dialog.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Config/Globals.hpp>
#include <Entities/BoardParser.hpp>

#include <Engine/Flow/StateManager.hpp>
#include <States/GameStateMainMenu.hpp>

GameStateGame::GameStateGame():
	game(NULL),
	willQuit(false)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load()
{
	try {
		this->game = new Game();
		this->game->start(Globals::Game::current_level);
		this->game->scores->load();
	}
	catch (BoardParserException& e)
	{
		Dialog::show("Couldn't load the level! (Error: \"" + e.message + "\")", true);
		this->willQuit = true;
	}
	catch (ScoreFileException& e)
	{
		// Show a non-intrusive dialog with why
		// we couldn't open high score file
		//e.message
	}
	catch (std::runtime_error& e)
	{
		// Some error happened during INI parsing...
		// What should we do?
	}
}
void GameStateGame::unload()
{
	SAFE_DELETE(this->game);
}
void GameStateGame::update()
{
	if (this->willQuit)
		StateManager::quit();

	this->game->handleInput();
	this->game->update();

	if (this->game->isOver())
	{
		// I'm touching a lot of different stuff
		// inside the update() function.
		// I know I shouldn't render things here.
		// Oh boy, this should be refactored away.
		this->game->scores->save();
		Utils::Time::delay_ms(500);

		this->game->draw();

		if (Dialog::askBool("Retry?", "Game Over", true))
			this->load(); // restart the game
		else
			StateManager::change(new GameStateMainMenu());
	}

	if (this->game->willQuit())
		this->willQuit = true;

	if (this->game->willReturnToMenu())
		StateManager::change(new GameStateMainMenu());
}
void GameStateGame::draw()
{
	if (! this->willQuit)
		this->game->draw();
}

