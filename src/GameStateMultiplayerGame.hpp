#ifndef GAMESTATEMULTIPLAYERGAME_H_DEFINED
#define GAMESTATEMULTIPLAYERGAME_H_DEFINED

#include "GameState.hpp"
#include "Sprite.hpp"
#include "Board.hpp"
#include "Snake.hpp"
#include "TimerCounter.hpp"
#include "FoodManager.hpp"

//#include "Music.hpp"

/// Everything that happens on the game.
class GameStateMultiplayerGame: public GameState
{
public:
    GameStateMultiplayerGame();
    virtual ~GameStateMultiplayerGame();

    /// Constructs everything necessary for the game.
    void load(int stack=0);

    /// Destroys anything builded during the game.
    int unload();

    /// Updates all possible things on the game.
    ///
    /// @return A status code indicating what to do (should we
    ///         change states/quit the game?). The codes are
    ///         defined on GameState.hpp.
    GameState::StateCode update(float dt);

    /// Shows everything onscreen;
    void render();

private:
//    Music* music;     ///< Menu's background music.
    Snake* player1;
    Snake* player2;

    Board* board;
    int boardX; /// Custom on-the-fly changeable board X position
    int boardY; /// Custom on-the-fly changeable board Y position.

    TimerCounter* gameTimer;
    FoodManager* foods;
};

#endif //GAMESTATEMULTIPLAYERGAME_H_DEFINED
