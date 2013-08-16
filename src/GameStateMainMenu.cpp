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
    // need to create a method to print a matrix of strings
    // or at least print a string divided with \n s

    std::string logoLongestLine = "888  888       888b 888  888 .d888888 888888K  88888888";
    int logoX = Ncurses::currentWidth/2 - logoLongestLine.length()/2;

    Ncurses::setStyle(Color::pair("green", "default"));
    Ncurses::print("         ,d8888b.                     888", logoX, 1);
    Ncurses::print("        d88P  Y88b                    888", logoX, 2);
    Ncurses::print("         Y88b.                        888", logoX, 3);
    Ncurses::print("88888b.   Y888b.    88888b.    8888b. 888  888  .d88b.", logoX, 4);
    Ncurses::print("888  88b     8Y88b. 888  88b      88b 888 .88P d8P  Y8b", logoX, 5);
    Ncurses::print("888  888       888b 888  888 .d888888 888888K  88888888", logoX, 6);
    Ncurses::print("888  888 Y88b  d88P 888  888 888  888 888  88b Y8b.", logoX, 7);
    Ncurses::print("888  888   Y8888P   888  888  Y888888 888  888  Y88888", logoX, 8);

    Ncurses::setStyle(Color::pair("red", "black"));
    Ncurses::print("Press <enter> to start", 20, 10);

//    Ncurses::print("Delta: " + Ncurses::intToString(Ncurses::getDelta()), 1, 1);
}

