#ifndef PLAYER_H_DEFINED
#define PLAYER_H_DEFINED

#include <Interface/Window.hpp>
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

	void move(Direction direction);
	void kill();

	void update();
	void draw(Window* win);

	bool collideWithItself();

private:
	std::vector<Body> body;

	bool alive;

	Direction currentDirection;
	Direction nextDirection;
};

#endif //PLAYER_H_DEFINED

