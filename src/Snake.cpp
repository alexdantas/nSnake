#include "Snake.hpp"
#include "Input.hpp"

Snake::Snake(Board* board, int x, int y):
    board(NULL),
    size(INITIAL_SIZE),
    previousDirection(INITIAL_DIRECTION),
    currentDirection(previousDirection)
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
    for (unsigned int i = 0; i < (this->body.size()); i++)
        if (this->body[i]) delete this->body[i];
}
void Snake::update()
{
    Input* input = Input::getInstance();

    if (input->isKeyDown('w'))
    {
        this->currentDirection = Snake::UP;
        this->move();
    }
}
void Snake::move(bool willIncrease)
{
    // Remember, we're assuming there are no constraints
    // on moving the snake.
    // They're checked on Snake::update

    // First we'll logically move the snake, then
    // we'll "physically" move it on the board

    // Before we even move the snake, we must
    // empty the position of the board where the last
    // piece currently is.
    // Note that if the player has eaten a food,
    // this will not happen and the snake will increase.
    if (!willIncrease)
        this->board->at(this->body[this->size - 1]->x,
                        this->body[this->size - 1]->y)->set(Tile::NOTHING);

    // All the pieces must move to the
    // position of the next one.
    for (int i = (this->size - 1); i > 0; i--)
    {
        this->body[i]->x = this->body[i - 1]->x;
        this->body[i]->y = this->body[i - 1]->y;
    }

    // And now we move the head according to the
    // current direction
    switch (this->currentDirection)
    {
    case Snake::UP:    this->body[0]->y -= 1; break;
    case Snake::DOWN:  this->body[0]->y += 1; break;
    case Snake::LEFT:  this->body[0]->x -= 1; break;
    case Snake::RIGHT: this->body[0]->x += 1; break;

    default: break;
    }

    // Finally, we refresh the board's contents
    // according to the snake's position.
    for (int i = 1; i < (this->size); i++)
        this->board->at(this->body[i]->x,
                        this->body[i]->y)->set(Tile::SNAKE);

    this->board->at(this->body[0]->x,
                    this->body[0]->y)->set(Tile::SNAKE_HEAD);

    // Saving this for the next frame
    this->previousDirection = this->currentDirection;
}

