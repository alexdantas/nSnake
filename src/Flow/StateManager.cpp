#include <Flow/StateManager.hpp>
#include <Flow/GameStateGame.hpp>
#include <Flow/GameStateMainMenu.hpp>
#include <Flow/InputManager.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>
#include <Config/YAMLFile.hpp>

StateManager::StateManager():
	currentState(NULL),
	sharedInfo(0)
{
	// The first state, Hardcoded
	this->currentState = new GameStateMainMenu();
	this->currentState->load();
}
StateManager::~StateManager()
{
	if (this->currentState)
		this->currentState->unload();

	SAFE_DELETE(this->currentState);
}
void StateManager::run()
{
	bool letsQuit = false;

	while (!letsQuit)
	{
		InputManager::update();

		// Updating the whole state.
		// This value is returned from it tell us if
		// we need to switch from the current state.
		GameState::StateCode whatToDoNow;

		whatToDoNow = this->currentState->update();

		switch (whatToDoNow)
		{
		case GameState::CONTINUE:
			// Just continue on the current state.
			break;

		case GameState::QUIT:
			this->currentState->unload();
			delete this->currentState;
			this->currentState = NULL;

			letsQuit = true;
			break;

		case GameState::GAME_START:
		{
			this->currentState->unload();
			delete this->currentState;

			this->currentState = new GameStateGame();
			this->currentState->load();
			break;
		}

		case GameState::MAIN_MENU:
		{
			this->currentState->unload();
			delete this->currentState;

			this->currentState = new GameStateMainMenu();
			this->currentState->load();
			break;
		}

		default:
			break;
		}

		if (this->currentState)
			this->currentState->draw();

		Utils::Time::delay_ms(100);
	}

	// // Right before quitting, we must save current
	// // user's settings
	// Globals::saveSettings();

	// // And set the current profile as the default
	// // to load next time.
	// INI ini;
	// if (! ini.load(Globals::Config::file))
	// 	ini.create();

	// ini.set("profiles:default", Globals::Profiles::current->name);
	// ini.save(Globals::Config::file);
}

