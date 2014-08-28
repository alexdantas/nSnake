#ifndef GAMESTATE_H_DEFINED
#define GAMESTATE_H_DEFINED

/// Abstract definition of a game state.
///
/// ## For dummies
///
/// A game state is a certain well-defined way the game behaves.
///
/// Like the main menu (a state), for example. From there,
/// the player can start the actual game (another state) and,
/// show the pause menu (even another state).
///
/// Examples include the game over screen, high score screen,
/// cutscenes, etc.
///
/// ## For developers
///
/// To switch from one GameState to another, use
/// `StateManager::change`, like this:
///
///     StateManager::change(new YourCustomGameState());
///
/// Don't worry, it cleans up the current state.
///
class GameState
{
public:
    // Left this here just because.
    virtual ~GameState() {};

    /// Where every state initializes it's resources.
    ///
    /// The *stack* is the previous state's returned value
    /// from *unload()*, allowing a state to communicate with
    /// the next one.
    virtual void load() = 0;

    /// Where every state destroys it's resources.
    ///
    /// The returned value will be sent to the next state's
    /// *load()* so we can send a specific message to it.
    virtual void unload() = 0;

    /// Called every frame, where states calculate everything
    /// that can change.
    ///
    /// The returned value will be checked by the *StateManager*
    /// to see if we must change the current state - if so,
    /// which one should we go next.
    virtual void update() = 0;

    /// Called every frame, where states draw stuff on screen.
    virtual void draw() = 0;
};

#endif //GAMESTATE_H_DEFINED

