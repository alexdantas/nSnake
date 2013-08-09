#include "GameStateMainMenu.hpp"
#include "Input.hpp"
#include "Ncurses.hpp"

GameStateMainMenu::GameStateMainMenu()
{ }
GameStateMainMenu::~GameStateMainMenu()
{ }
void GameStateMainMenu::load(int stack)
{
    UNUSED(stack);
}
int GameStateMainMenu::unload()
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
GameState::StateCode GameStateMainMenu::update(float dt)
{
    UNUSED(dt);

    Input* input = Input::getInstance();
    input->update();

    if (input->isKeyDown('q'))
        return GameState::QUIT;

    if (input->isKeyDown('\n'))
        return GameState::GAME_START;

    return GameState::CONTINUE;
}
void GameStateMainMenu::render()
{
    Ncurses::print("this is awesome", 20, 10);

//    Ncurses::print("Delta: " + Ncurses::intToString(Ncurses::getDelta()), 1, 1);
}

