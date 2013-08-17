#ifndef GAMESTATEGAME_H_DEFINED
#define GAMESTATEGAME_H_DEFINED

#include "GameState.hpp"
#include "Sprite.hpp"
#include "Board.hpp"
#include "Snake.hpp"
#include "TimerCounter.hpp"
#include "FoodManager.hpp"

//#include "Music.hpp"

/// Everything that happens on the game.
class GameStateGame: public GameState
{
public:
    GameStateGame();
    virtual ~GameStateGame();

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
    Snake* player;
    Board* board;

    TimerCounter* gameTimer;
    FoodManager* foods;
};

#endif //GAMESTATEGAME_H_DEFINED
