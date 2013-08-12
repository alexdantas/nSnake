#include "Snake.hpp"

Snake::Snake(Board* board, int x, int y):
    board(NULL)

{
    if ((x > board->getWidth()) ||
        (y > board->getHeight()))
        throw "Cannot place Snake on Board - invalid position.";

    this->board = board;
    this->body.resize(INITIAL_SIZE);

    for (int i = 0; i < INITIAL_SIZE; i++)
        this->body[i] = new SnakeBody(x, y);

    if (this->board->at(x, y)->isEmpty())
        this->board->at(x, y)->set(Tile::SNAKE_HEAD);
    else
        throw "Snake cannot be placed on a non-empty tile.";
}
Snake::~Snake()
{
    for (int i = 0; i < INITIAL_SIZE; i++)
        if (this->body[i]) delete this->body[i];
}
void Snake::update()
{

}

