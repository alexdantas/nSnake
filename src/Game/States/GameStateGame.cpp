#include <Flow/GameStateGame.hpp>
#include <Flow/StateManager.hpp>
#include <Misc/Utils.hpp>
#include <Interface/Dialog.hpp>
#include <Interface/Ncurses.hpp>
#include <Config/Globals.hpp>
#include <Game/BoardParser.hpp>

GameStateGame::GameStateGame():
	game(NULL),
	willQuit(false)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int stack)
{
	UNUSED(stack);

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
int GameStateGame::unload()
{
	SAFE_DELETE(this->game);

	return 0;
}
GameState::StateCode GameStateGame::update()
{
	if (this->willQuit)
		return GameState::QUIT;

	this->game->handleInput();
	this->game->update();

	if (this->game->isOver())
	{
		// I'm touching a lot of different stuff
		// inside the update() function.
		// I know I shouldn't render things here.
		// Oh boy, this should be refactored away.
		this->game->scores->save();
		Ncurses::delay_ms(500);

		this->game->draw();

		if (Dialog::askBool("Retry?", "Game Over", true))
			this->load(); // restart the game
		else
			return GameState::MAIN_MENU;
	}

	if (this->game->willQuit())
		this->willQuit = true;

	if (this->game->willReturnToMenu())
		return GameState::MAIN_MENU;

	return GameState::CONTINUE;
}
void GameStateGame::draw()
{
	if (! this->willQuit)
		this->game->draw();
}

