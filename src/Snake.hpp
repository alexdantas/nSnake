#ifndef SNAKE_H_DEFINED
#define SNAKE_H_DEFINED

#include "Board.hpp"

/// Each piece of the snake body.
struct SnakeBody
{
    SnakeBody(int x, int y):
        x(x), y(y)
    { }

    int x; /// X position
    int y; /// Y position
};

/// The initial size of every snake.
#define INITIAL_SIZE 3

/// The initial direction of every snake
#define INITIAL_DIRECTION Snake::RIGHT

/// The snake controlled by the player.
///
class Snake
{
public:
    /// All the directions the snake can move.
    enum Directions { UP, DOWN, LEFT, RIGHT };

    /// Creates a snake at *board* on *x* *y*.
    Snake(Board* board, int x, int y);

    virtual ~Snake();

    /// Updates the snake input.
    void update();

    /// Commits the snake movement based on *currentDirection*.
    ///
    /// *currentDirection* gets changed by the player based on
    /// input.
    /// This method actually moves the snake based on that and
    /// sets it on *previousDirection*.
    void move();

    /// Tells if this snake is alive.
    bool isAlive();

    /// Does all the effects of eating a fruit.
    ///
    /// Makes the snake grow and affects score.
    void eatFood();

private:
    /// Tells if this snake is alive.
    bool alive;

    /// The board where this snake is inserted.
    Board* board;

    /// All the pieces of the snake, including the head.
    std::vector<SnakeBody*> body;

    /// The snake's body current size.
    int size;

    /// This snake's current score.
    unsigned int score;

    /// The direction the snake had right at the start of this
    /// frame.
    Snake::Directions previousDirection;

    /// The direction the snake currently has, modifiable by
    /// the player.
    ///
    /// When this frame ends, this direction will be commited
    /// to *previousDirection*.
    Snake::Directions currentDirection;

    /// Tells if at this frame the snake has eaten a fruit.
    ///
    /// In this case, it will grow up.
    bool eatenFood;
};

#endif //SNAKE_H_DEFINED

