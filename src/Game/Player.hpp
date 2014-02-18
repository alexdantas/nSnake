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

	void move(Direction direction);

	void update();
	void draw(Window* win);

private:
	std::vector<Body> body;
	bool alive;
	Direction currentDirection;
};

#endif //PLAYER_H_DEFINED

