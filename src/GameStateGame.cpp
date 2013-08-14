#include "GameStateGame.hpp"
#include "Input.hpp"
#include "Ncurses.hpp"

GameStateGame::GameStateGame():
    player(NULL),
    board(NULL),
    gameTimer(NULL)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int stack)
{
    UNUSED(stack);

    this->board = new Board(80, 24, false);
    this->board->at(2, 2).set(Tile::FOOD);
    this->board->at(1, 2).set(Tile::FOOD);
    this->board->at(9, 3).set(Tile::WALL);

    this->player = new Snake(this->board, 5, 5);

    // Again, this is the game speed.
    // This is the timer that says when the snake will be
    // updated.
    this->gameTimer = new TimerCounter(100);
    this->gameTimer->startCounting();
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

    safe_delete(this->player);
    safe_delete(this->board);
    return 0;
}
GameState::StateCode GameStateGame::update(float dt)
{
    UNUSED(dt);

    Input* input = Input::getInstance();
    input->update();

    if (input->isKeyDown('q')) // quit the game
        return GameState::QUIT;

    if (input->isKeyDown('r')) // restart the game!
        return GameState::GAME_START;

    if (input->isKeyDown('i')) // restart the game!
        this->player->eatFood();

    this->player->update();

    // If the game speed time has passed, we'll force
    // the snake to move.
    //
    // Note that we always check for input on Snake::update(),
    // so don't worry about that.
    if (this->gameTimer->isDone())
    {
        this->player->move();
        this->player->checkCollision();
        this->gameTimer->startCounting();
    }

    return GameState::CONTINUE;
}
void GameStateGame::render()
{
    Ncurses::setStyle(Color::pair("yellow", "black"));
    Ncurses::print("This is where the game would be.", 30, 2);
    Ncurses::print("Just wait a while, things are being.", 30, 3);
    Ncurses::print("actively developed.", 30, 4);

    Ncurses::setStyle(Color::pair("magenta", "black"));
    Ncurses::print("Press <q> to quit.", 30, 6);

    this->board->render(0, 0);
}

