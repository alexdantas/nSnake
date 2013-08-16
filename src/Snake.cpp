#include "Snake.hpp"
#include "Input.hpp"
#include "Shapes.hpp"

int Snake::playerCount = 0;


Controls::Controls():
    up(KEY_UP), down(KEY_DOWN), left(KEY_LEFT), right(KEY_RIGHT)
{ }

Controls::Controls(int up, int down, int left, int right):
    up(up), down(down), left(left), right(right)
{ }

Snake::Snake(Board* board):
    alive(true),
    board(NULL),
    size(INITIAL_SIZE),
    previousDirection(INITIAL_DIRECTION),
    currentDirection(previousDirection),
    eatenFood(false)
{
    // Setting up players 0, 1, 2 or 3
    this->player = Snake::playerCount;
    Snake::playerCount++;

    // I KNOW this is a hell of a bad practice,
    // but I've copied and pasted the two constructors,
    //

    /// TODO I NEED to find a way to fix them constructors

    this->board = board;
    int x = this->board->getLevel()->getStartingX(this->player);
    int y = this->board->getLevel()->getStartingY(this->player);

    if ((x > board->getWidth()) ||
        (y > board->getHeight()))
        throw "Cannot place Snake on Board - invalid position.";


    this->body.resize(INITIAL_SIZE);

    for (int i = 0; i < INITIAL_SIZE; i++)
        this->body[i] = SnakeBody(x, y);

    if (this->board->at(x, y).isEmpty())
        this->board->at(x, y).set(Tile::SNAKE_HEAD);
    else
        throw "Snake cannot be placed on a non-empty tile.";

    this->control = Controls('w', 's', 'a', 'd');
}
Snake::Snake(Board* board, int x, int y):
    alive(true),
    board(NULL),
    size(INITIAL_SIZE),
    previousDirection(INITIAL_DIRECTION),
    currentDirection(previousDirection),
    eatenFood(false)
{
    if ((x > board->getWidth()) ||
        (y > board->getHeight()))
        throw "Cannot place Snake on Board - invalid position.";

    this->board = board;
    this->body.resize(INITIAL_SIZE);

    for (int i = 0; i < INITIAL_SIZE; i++)
        this->body[i] = SnakeBody(x, y);

    if (this->board->at(x, y).isEmpty())
        this->board->at(x, y).set(Tile::SNAKE_HEAD);
    else
        throw "Snake cannot be placed on a non-empty tile.";
}
Snake::~Snake()
{ }
void Snake::update()
{
    if (!(this->alive))
        return;

    Input* input = Input::getInstance();

    if (input->isKeyDown(this->control.up))
        if (this->previousDirection != Snake::DOWN)
            this->currentDirection = Snake::UP;

    if (input->isKeyDown(this->control.down))
        if (this->previousDirection != Snake::UP)
            this->currentDirection = Snake::DOWN;

    if (input->isKeyDown(this->control.left))
        if (this->previousDirection != Snake::RIGHT)
            this->currentDirection = Snake::LEFT;

    if (input->isKeyDown(this->control.right))
        if (this->previousDirection != Snake::LEFT)
            this->currentDirection = Snake::RIGHT;
}
void Snake::move()
{
    if (!(this->alive))
        return;

    // If we uncomment this, we turn our game into TRON!
    // How awesome is that?
//    this->eatFood();

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
    if (!(this->eatenFood))
    {
//        this->board->at(this->body[this->size - 1].x,
//                        this->body[this->size - 1].y).clear();
        this->board->at(this->body[this->size - 1].x,
                        this->body[this->size - 1].y).remove(Tile::SNAKE_BODY);
    }
    else
        this->eatenFood = false; // by doing this we visually increase
                                 // the snake size on the screen!

    // All the pieces must move to the
    // position of the next one.
    for (int i = (this->size - 1); i > 0; i--)
    {
        this->body[i].x = this->body[i - 1].x;
        this->body[i].y = this->body[i - 1].y;
    }

    // And now we move the head according to the
    // current direction
    switch (this->currentDirection)
    {
    case Snake::UP:    this->body[0].y -= 1; break;
    case Snake::DOWN:  this->body[0].y += 1; break;
    case Snake::LEFT:  this->body[0].x -= 1; break;
    case Snake::RIGHT: this->body[0].x += 1; break;

    default: break;
    }

    // Here's the collision test with the board extremes.
    // On our case we can both be killed or be teleported
    // when collided.
    if (this->board->at(this->body[0].x,
                        this->body[0].y).has(Tile::BORDER))
    {
        // Dies if collided with board extremes.
        // Don't call this->die() on purpose!
        this->alive = false;
    }

    if (this->board->at(this->body[0].x,
                        this->body[0].y).has(Tile::TELEPORT_BORDER))
    {
        this->teleport();
    }

    // Finally, we refresh the board's contents
    // according to the snake's position.
    for (int i = 1; i < (this->size); i++)
        this->board->at(this->body[i].x,
                        this->body[i].y).add(Tile::SNAKE_BODY);

    // Removing the previous head
    this->board->at(this->body[1].x,
                    this->body[1].y).remove(Tile::SNAKE_HEAD);

    if (this->alive)
        this->board->at(this->body[0].x,
                        this->body[0].y).add(Tile::SNAKE_HEAD);
    else
        this->board->at(this->body[0].x,
                        this->body[0].y).set(Tile::SNAKE_DEAD_HEAD);

    // Saving this for the next frame
    this->previousDirection = this->currentDirection;
}
bool Snake::isAlive()
{
    return (this->alive);
}
void Snake::eatFood()
{
    this->score++; // whatever
    this->eatenFood = true;

    // Add new body part
    SnakeBody body(this->body[this->size - 1].x,
                   this->body[this->size - 1].y);
    this->body.push_back(body);
    this->size++;
}
void Snake::die()
{
    if (!(this->alive))
        return;

    this->alive = false;
    this->board->at(this->body[0].x,
                    this->body[0].y).set(Tile::SNAKE_DEAD_HEAD);
}

void Snake::checkCollision()
{
    int headX = this->body[0].x;
    int headY = this->body[0].y;

    // DOESNT WORK
//    Tile headTile = this->board->at(headX, headY);

    if (this->board->at(headX, headY).has(Tile::FOOD))
        this->eatFood();

    if (this->board->at(headX, headY).has(Tile::WALL) ||
        this->board->at(headX, headY).has(Tile::SNAKE_BODY))
        this->die();
}
int Snake::getScore()
{
    return (this->score);
}
void Snake::teleport()
{
    Point valid, border;

    switch (this->currentDirection)
    {
    case Snake::RIGHT:
        valid  = Point((this->body[0].x - 1), this->body[0].y);
        border = valid;

        while (!(this->board->at(border.x, border.y).has(Tile::TELEPORT_BORDER)))
        {
            border.x--;

            if (!(this->board->at(border.x, border.y).isInvalidForPlayer()))
                valid = border;
        }

        break;
    case Snake::LEFT:
        valid  = Point((this->body[0].x + 1), this->body[0].y);
        border = valid;

        while (!(this->board->at(border.x, border.y).has(Tile::TELEPORT_BORDER)))
        {
            border.x++;

            if (!(this->board->at(border.x, border.y).isInvalidForPlayer()))
                valid = border;
        }

        break;

    case Snake::UP:
        valid  = Point(this->body[0].x, (this->body[0].y + 1));
        border = valid;

        while (!(this->board->at(border.x, border.y).has(Tile::TELEPORT_BORDER)))
        {
            border.y++;

            if (!(this->board->at(border.x, border.y).isInvalidForPlayer()))
                valid = border;
        }
        break;

    case Snake::DOWN:
        valid  = Point(this->body[0].x, (this->body[0].y - 1));
        border = valid;

        while (!(this->board->at(border.x, border.y).has(Tile::TELEPORT_BORDER)))
        {
            border.y--;

            if (!(this->board->at(border.x, border.y).isInvalidForPlayer()))
                valid = border;
        }
        break;
    }
    this->body[0].x = valid.x;
    this->body[0].y = valid.y;
}
void Snake::setKeys(int up, int down, int left, int right)
{
    this->control = Controls(up, down, left, right);
}

