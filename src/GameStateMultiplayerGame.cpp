#include "GameStateMultiplayerGame.hpp"
#include "Input.hpp"
#include "Ncurses.hpp"
#include "Config.hpp"

GameStateMultiplayerGame::GameStateMultiplayerGame():
    player1(NULL),
    player2(NULL),
    board(NULL),
    gameTimer(NULL),
    foods(NULL)
{ }
GameStateMultiplayerGame::~GameStateMultiplayerGame()
{ }
void GameStateMultiplayerGame::load(int stack)
{
    UNUSED(stack);

    this->boardX = 0;
    this->boardY = 1;
    this->board = new Board(80, 23);
    this->board->setBorders(true);

    bool result = this->board->loadFile("levels/arena.nsnake");
    if (!result)
        throw "GameStateMultiplayerGame: Couldn't load the level.";

    if (this->board->getSupportedPlayers() < 2)
        throw "GameStateGame: Not a multiplayer level.";

    if (Config::centerGameScreenHorizontally)
        this->boardX = Ncurses::currentWidth/2 - this->board->getWidth()/2;

    if (Config::centerGameScreenVertically)
        this->boardY = Ncurses::currentHeight/2 - this->board->getHeight()/2;

    this->foods = new FoodManager(this->board);
    this->foods->addAtRandom();

    this->player1 = new Snake(this->board);
    this->player1->setKeys('w', 's', 'a', 'd');

    this->player2 = new Snake(this->board, 30, 7);
    this->player2->setKeys(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);

    // Again, this is the game speed.
    // This is the timer that says when the snake will be
    // updated.
    this->gameTimer = new TimerCounter(100);
    this->gameTimer->startCounting();
}
int GameStateMultiplayerGame::unload()
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

    safe_delete(this->player1);
    safe_delete(this->player2);
    safe_delete(this->board);
    return 0;
}
GameState::StateCode GameStateMultiplayerGame::update(float dt)
{
    UNUSED(dt);

    Input* input = Input::getInstance();
    input->update();

    if (input->isKeyDown('q')) // quit the game
        return GameState::QUIT;

    if (input->isKeyDown('r')) // restart the game!
        return GameState::GAME_START;

    this->player1->update();
    this->player2->update();

    // If the game speed time has passed, we'll force
    // the snakes to move.
    //
    // Note that we always check for input on Snake::update(),
    // so don't worry about that.
    if (this->gameTimer->isDone())
    {
        this->player1->move();
        this->player1->checkCollision();

        this->player2->move();
        this->player2->checkCollision();

        this->foods->update();
        this->gameTimer->startCounting();
    }

    return GameState::CONTINUE;
}
void GameStateMultiplayerGame::render()
{
    std::string logo("nSnake v2.0");

    Ncurses::setStyle(Color::pair("cyan"));
    Ncurses::print(logo, 0, 0);

    std::string level("level");
    int levelX = (Ncurses::currentWidth/8);

    Ncurses::print(level, levelX, 0);
    Ncurses::print(Ncurses::intToString(2),
                   levelX + 3 + level.length(), 0);

    // std::string score("score");
    // int scoreX = (Ncurses::currentWidth/8 * 2);

    // Ncurses::print(score, scoreX, 0);
    // Ncurses::print(Ncurses::intToString(this->player->getScore()),
    //                scoreX + 3 + score.length(), 0);

    std::string info("| <q> quit | <r> restart |");
    int infoX = Ncurses::currentWidth - info.length();

    Ncurses::setStyle(Color::pair("magenta"));
    Ncurses::print(info, infoX, 0);

    this->board->render(this->boardX, this->boardY);
}

