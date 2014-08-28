#include <Engine/Flow/StateManager.hpp>
#include <Engine/InputManager.hpp>
#include <Engine/Helpers/Utils.hpp>

#include <States/GameStateGame.hpp>
#include <States/GameStateMainMenu.hpp>

void StateManager::change(GameState* newState)
{
	// Yeah, right!
	//
	// My solution to immediately change from one
	// state to another is to launch an exception!
	//
	// See in action on `StateManager::run()`
	//
	// It seems kinda hackish, but at least it makes
	// StateManager independent of all user-defined
	// GameStates...
	throw StateManagerChangeException(newState);
}
void StateManager::quit()
{
	// Same thing as the function above
	throw StateManagerQuitException();
}



StateManager::StateManager(GameState* initialState):
	currentState(initialState),
	sharedInfo(0)
{
	if (! this->currentState)
		throw "No state given to StateManager";

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

	while (! letsQuit)
	{
		try
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

			default:
				break;
			}

			if (this->currentState)
				this->currentState->draw();

			Utils::Time::delay_ms(100);
		}

		// Special type of exception used to
		// instantaneously change from one state
		// to another.
		catch (StateManagerChangeException& e)
		{
			this->currentState->unload();
			SAFE_DELETE(this->currentState);

			this->currentState = e.newState;
			this->currentState->load();

			// Continue with the loop
		}
		catch (StateManagerQuitException& e)
		{
			this->currentState->unload();
			SAFE_DELETE(this->currentState);

			break; // out of the loop
		}
		// All other exceptions will keep going up
	}
}

