#include "GameStateGame.hpp"
#include "Input.hpp"
#include "Ncurses.hpp"

GameStateGame::GameStateGame()
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int stack)
{
    UNUSED(stack);
}
int GameStateGame::unload()
{
// Time to delete!
// This macro deletes a thing only if it's non-NULL,
// making sure it won't double-delete things.
#define safe_delete(x) \
{                      \
    if (x)             \
    {                  \
        delete (x);    \
        x = NULL;      \
    }                  \
}

    return 0;
}
GameState::StateCode GameStateGame::update(float dt)
{
    UNUSED(dt);

    Input* input = Input::getInstance();
    input->update();

    if (input->isKeyDown('q'))
        return GameState::QUIT;

    if (input->isKeyDown(KEY_ENTER))
        return GameState::GAME_START;

    return GameState::CONTINUE;
}
void GameStateGame::render()
{
    Ncurses::setStyle(Color::pair("yellow", "cyan"));
    Ncurses::print("this is GAEM", 30, 30);
}

