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
/// All the other game states inherit from this baby.
/// Each one must implement at least four methods - *load()*,
/// *unload()*, *update()* and *draw()*.
///
/// Each state controls the game flow via return values of
/// the *update()* method. If, for instance, the player dies
/// during the game, it should return *GAME_OVER* or *QUIT*.
///
/// A state can also comunicate with the next one by returning
/// a value from *unload()*. That value will be passed on the
/// next state's *load()* and it should know what to do with it.
///
class GameState
{
public:
    /// All possible transitions between states.
    ///
    /// They are used to change from one state to the other.
    /// The current state returns this at *update()* and the
    /// *StateManager* makes the appropriate changes.
    enum StateCode
    {
	    // Internal codes for quitting and continuing
        QUIT, CONTINUE,

        // The actual game screens
        MAIN_MENU,
        GAME_START,
        GAME_OVER
    };

    // Left this here just because.
    virtual ~GameState() {};

    /// Where every state initializes it's resources.
    ///
    /// The *stack* is the previous state's returned value
    /// from *unload()*, allowing a state to communicate with
    /// the next one.
    virtual void load(int stack=0) = 0;

    /// Where every state destroys it's resources.
    ///
    /// The returned value will be sent to the next state's
    /// *load()* so we can send a specific message to it.
    virtual int unload() = 0;

    /// Called every frame, where states calculate everything
    /// that can change.
    ///
    /// The returned value will be checked by the *StateManager*
    /// to see if we must change the current state - if so,
    /// which one should we go next.
    virtual StateCode update() = 0;

    /// Called every frame, where states draw stuff on screen.
    virtual void draw() = 0;

private:
};

#endif //GAMESTATE_H_DEFINED

