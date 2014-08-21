#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED

#include <Engine/Graphics/Window.hpp>

#include <vector>

struct Body
{
	int x;
	int y;

	Body(int x, int y):
		x(x),
		y(y)
	{ }
};

// Avoiding circular #include hell.
class Board;

///
class Player
{
public:
	enum Direction
	{
		UP, DOWN, LEFT, RIGHT
	};

	Player(int x, int y);
	virtual ~Player() { };

	bool isAlive();
	int getSize();

	int getX(); ///< Returns the head's x position.
	int getY(); ///< Returns the head's y position.

	void moveTo(int x, int y);

	void move(Direction direction);
	void kill();

	void update(Board* board);
	void draw(Window* win);

	bool headHit(int x, int y);

	/// Tells if something at #x and #y collides with
	/// any part of the snake.
	///
	/// @note #isHead is a huge HACK to allow me to
	///       check if the head collides with the body.
	///       Ignore it.
	bool bodyHit(int x, int y, bool isCheckingHead=false);

	void increase();

private:
	std::vector<Body> body;

	bool alive;

	Direction currentDirection;
	Direction nextDirection;
};

#endif //PLAYER_H_DEFINED

