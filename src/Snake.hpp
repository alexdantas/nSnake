#ifndef SNAKE_H_DEFINED
#define SNAKE_H_DEFINED

#include "Board.hpp"

/// Each piece of the snake body.
struct SnakeBody
{
    SnakeBody(int x, int y):
        x(x), y(y)
    { }

    int x;
    int y;
};

/// The initial size of every snake.
#define INITIAL_SIZE 3

/// The snake controlled by the player.
///
class Snake
{
public:
    /// Creates a snake at *board* on *x* *y*.
    Snake(Board* board, int x, int y);

    virtual ~Snake();

    void update();

private:
    /// The board where this snake is inserted.
    Board* board;

    /// All the pieces of the snake, including the head.
    std::vector<SnakeBody*> body;

    /// This snake's current score.
    unsigned int score;
};

#endif //SNAKE_H_DEFINED

