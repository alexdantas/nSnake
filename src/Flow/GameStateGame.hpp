#ifndef GAMESTATEGAME_H_DEFINED
#define GAMESTATEGAME_H_DEFINED

#include <Flow/GameState.hpp>
#include <Game/Game.hpp>

/// This represents the actual game taking place.
///
/// Here is defined all the game logic and rules.
///
/// ## For developers:
///
/// Before you continue, take a deep breath.
///
/// Done? OK.
/// This is the most complex class of this game and it uses
/// pretty much every other class on the project.
///
/// To understand it you must have a lot of patience and that
/// "I'll see what it means later" feeling.
///
/// Don't try to understand everything at once!
/// Try to abstract a litte and follow the code thinking only in
/// terms of what you think it's doing.
///
/// After you've quite guessed what a method does, start looking
/// into the classes that it uses.
///
class GameStateGame: public GameState
{
public:
	GameStateGame();
    virtual ~GameStateGame();

    /// Constructs everything necessary for the game.
    void load(int stack=0);

    /// Destroys anything builded during the game
    int unload();

    /// Updates all possible things on the game.
    ///
    /// @return A status code indicating what to do (should we
    ///         change states/quit the game?). The codes are
    ///         defined on GameState.hpp.
	GameState::StateCode update();

    /// Shows everything onscreen;
    void draw();

private:
	/// The rules of the current game.
	Game* game;

	/// Keeps the game running.
	bool willQuit;
};

#endif //GAMESTATEGAME_H_DEFINED

