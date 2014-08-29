#ifndef STATEMANAGER_H_DEFINED
#define STATEMANAGER_H_DEFINED

#include <Engine/Flow/GameState.hpp>
#include <exception>

/// Custom exception that's used to instantly
/// change from one state to another.
///
class StateManagerChangeException : public std::exception
{
public:
	StateManagerChangeException(GameState* newState):
		newState(newState)
	{ }

	GameState* newState;
};

/// Custom exception for the specific case of
/// quitting the game.
///
/// Quile like the custom exception above.
///
class StateManagerQuitException : public std::exception
{
public:
	StateManagerQuitException()
	{ }
};

/// Controls all game states.
///
/// This is a very important class.
/// It contains the main game loop (
///
/// It makes the transitions between GameStates, assuring
/// they are properly cleaned and initialized.
///
class StateManager
{
public:
	/// Immediately changes to #newState
	///
	/// @note Don't worry, it cleans up the current
	///       GameState before.
	///
	static void change(GameState* newState);

	/// Immediately quits the game.
	///
	/// @note Don't worry, it cleans up the current
	///       GameState before.
	///
	static void quit();

	StateManager();
	virtual ~StateManager();

	/// Initializes pretty much everything,
	/// setting #initialState to run first.
	///
	/// Main entry point to the game and game loop.
	///
	/// This is where it all happens. The game never leaves this
	/// method, the only thing that's allowed to happen are
	/// state-specific methods called inside here.
	///
	/// If we leave this method, the game quits, as seen on
	/// *main.cpp*.
	void run(GameState* initialState);

private:
	GameState* currentState;
};

#endif /* STATEMANAGER_H_DEFINED */

