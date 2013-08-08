#ifndef STATEMANAGER_H_DEFINED
#define STATEMANAGER_H_DEFINED

#include "GameState.hpp"

/// Giga-class that handles all game states.
///
/// It makes the transitions between them, assuring each one is
/// properly initialized.
///
/// ## For developers:
///
/// Short and simple explanation:
///
/// * Creates the first state (allocating everything).
/// * Run it (updating and rendering).
/// * Whenever the state feels like changing, it will tell us
///   (quit, for example).
/// * Then we must delete the current state and repeat this whole
///   process for the next one.
///
class StateManager
{
public:
    /// Initializes pretty much everything.
    ///
    /// Here's all the important initializations (like the graphics
    /// scheme (SDL) or reading from the configuration file or
    /// *srand()*).
    StateManager(int width, int height);

    virtual ~StateManager();

    /// Main entry point and game loop.
    ///
    /// This is where it all happens. The game never leaves this
    /// method, the only thing that's allowed to happen are
    /// state-specific methods called inside here.
    ///
    /// If we leave this method, the game quits, as seen on
    /// *main.cpp*.
    void run();

private:
    /// Current game state - defines what will actually happen.
    GameState* currentState;

    /// Shared information between states.
    ///
    /// If a state want to share something with another, it should
    /// return a value that will be stored right here.
    ///
    /// Perhaps I should make this a template class or something.
    int sharedInfo;
};

#endif /* STATEMANAGER_H_DEFINED */

