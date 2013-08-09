#include <cstdlib> // srand()
#include <ctime>   // time()
#include "StateManager.hpp"
#include "Ncurses.hpp"
#include "Log.hpp"
#include "GameStateMainMenu.hpp"
#include "GameStateGame.hpp"

StateManager::StateManager(int width, int height)
{
    // Config::load("config.ini");

    // if (Config::debugMode)
    //     Log::debugMode(true);

    Ncurses::init(width, height); // framerate
    Ncurses::hideCursor(true);
    Ncurses::inputDelay(0);

    // Window::init(width, height, "Terminus", "terminus");
    // Graphics::init(Window::screen);

    Log::logToFiles();
    Log::debugMode(true);

    // For even further debugging purposes
//    Log::verboseMode(true);

    this->currentState = new GameStateMainMenu();

    this->currentState->load();
    this->sharedInfo = 0;

    srand(time(NULL));
}
StateManager::~StateManager()
{
    Ncurses::exit();

    if (this->currentState)
    {
        this->currentState->unload();
        delete this->currentState;

        this->currentState = NULL; // you never know
    }
}
void StateManager::run()
{
    bool letsQuit = false;

    bool firstFrame = true;

    while (!letsQuit)
    {
        // How many seconds have passed between last
        // frame and this one?
        float delta = Ncurses::getDeltaSeconds();

        // Little hack to avoid things jumping around on the
        // first frame.
        if (firstFrame)
        {
            delta = 1000.0/30.0;
            firstFrame = false;
        }

        // Just a reminder that every state handles input
        // individually, so no need to do that here.

        // Updating the whole state.
        // This value is returned from it tell us if
        // we need to switch from the current state.
        GameState::StateCode whatToDoNow = this->currentState->update(delta);

        switch (whatToDoNow)
        {
        case GameState::CONTINUE:
            // Just continue on the current state.
            break;

        case GameState::QUIT:
            letsQuit = true;
            break;

        case GameState::GAME_START: // yay, the actual game!
            this->sharedInfo = this->currentState->unload();
            delete (this->currentState);

            this->currentState = new GameStateGame();
            this->currentState->load(this->sharedInfo);
            break;

        default:
            break;
        }

        // Printing everything on the screen.
        Ncurses::clearScreen();
        this->currentState->render();

        Ncurses::refresh();

        // Let's wait a bit if the framerate is too low.
        Ncurses::framerateWait();
    }
}

