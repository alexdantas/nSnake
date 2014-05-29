#include <Flow/GameStateGame.hpp>
#include <Flow/StateManager.hpp>
#include <Misc/Utils.hpp>
#include <Interface/Dialog.hpp>
#include <Interface/Ncurses.hpp>
#include <Config/Globals.hpp>

GameStateGame::GameStateGame():
	game(NULL),
	willQuit(false)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int stack)
{
	UNUSED(stack);

	Score::loadFile();

	this->game = new Game();
	this->game->start();
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
		Score::saveFile();
		Ncurses::delay_ms(500);

		this->game->draw();

		if (Dialog::askBool("Retry?", "Game Over", true))
			this->game->start();
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
	this->game->draw();
}

