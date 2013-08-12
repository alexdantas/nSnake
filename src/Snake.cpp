#include "Snake.hpp"
#include "Input.hpp"

Snake::Snake(Board* board, int x, int y):
    alive(true),
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
    if (!(this->alive))
        return;

    Input* input = Input::getInstance();

    if (input->isKeyDown('w') ||
        input->isKeyDown(KEY_UP)) // make h,j,k,l
        if (this->previousDirection != Snake::DOWN)
            this->currentDirection = Snake::UP;

    if (input->isKeyDown('s') ||
        input->isKeyDown(KEY_DOWN))
        if (this->previousDirection != Snake::UP)
            this->currentDirection = Snake::DOWN;

    if (input->isKeyDown('a') ||
        input->isKeyDown(KEY_LEFT))
        if (this->previousDirection != Snake::RIGHT)
            this->currentDirection = Snake::LEFT;

    if (input->isKeyDown('d') ||
        input->isKeyDown(KEY_RIGHT))
        if (this->previousDirection != Snake::LEFT)
            this->currentDirection = Snake::RIGHT;
}
void Snake::move(bool willIncrease)
{
    if (!(this->alive))
        return;

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

    // Here's the collision test with the board extremes.
    // On our case we can both be killed or be teleported
    // when collided.
    if (this->board->hasBorders())
    {
        // Dies if collided with board extremes.
        if ((this->body[0]->x < 1) ||
            (this->body[0]->x > this->board->getWidth() - 2) ||
            (this->body[0]->y < 1) ||
            (this->body[0]->y > this->board->getHeight() - 2))
            this->alive = false;
    }
    else
    {
        // Teleports if collided with board extremes.
        if (this->body[0]->x < 1)
            this->body[0]->x = this->board->getWidth() - 2;

        if (this->body[0]->x > this->board->getWidth() - 2)
            this->body[0]->x = 1;

        if (this->body[0]->y < 1)
            this->body[0]->y = this->board->getHeight() - 2;

        if (this->body[0]->y > this->board->getHeight() - 2)
            this->body[0]->y = 1;
    }

    // Finally, we refresh the board's contents
    // according to the snake's position.
    for (int i = 1; i < (this->size); i++)
        this->board->at(this->body[i]->x,
                        this->body[i]->y)->set(Tile::SNAKE_BODY);

    if (this->alive)
        this->board->at(this->body[0]->x,
                        this->body[0]->y)->set(Tile::SNAKE_HEAD);
    else
        this->board->at(this->body[0]->x,
                        this->body[0]->y)->set(Tile::SNAKE_DEAD_HEAD);

    // Saving this for the next frame
    this->previousDirection = this->currentDirection;
}
bool Snake::isAlive()
{
    return (this->alive);
}

