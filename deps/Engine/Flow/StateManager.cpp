#include <Engine/Flow/StateManager.hpp>
#include <Engine/InputManager.hpp>
#include <Engine/Helpers/Utils.hpp>

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



StateManager::StateManager():
	currentState(NULL)
{ }
StateManager::~StateManager()
{
	if (this->currentState)
		this->currentState->unload();

	SAFE_DELETE(this->currentState);
}
void StateManager::run(GameState* initialState)
{
	if (! initialState)
		throw "No initial state given to StateManager";

	this->currentState = initialState;
	this->currentState->load();

	// Oohh yeah, the main game loop!
	while (true)
	{
		try
		{
			InputManager::update();

			this->currentState->update();

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

			break;
			// Quit out of the loop
		}

		// All other exceptions will keep going up
	}
}

