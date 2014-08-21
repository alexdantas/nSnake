#ifndef ANIMATIONSNAKES_H_DEFINED
#define ANIMATIONSNAKES_H_DEFINED

#include <Engine/Graphics/Animation.hpp>
#include <Engine/Helpers/Timer.hpp>

#include <vector>

#define MAX_SNAKES 100

struct LilSnake
{
	int x;
	int y;
	int size;

	LilSnake(int x, int y, int size):
		x(x),
		y(y),
		size(size)
	{ }
};

/// Rules and behavior of the Fire animation.
class AnimationSnakes: public Animation
{
public:
	AnimationSnakes(Window* window);
	virtual ~AnimationSnakes() {};

	void load();
	void update();
	void draw();

private:
	std::vector<LilSnake> lilsnakes;

	/// Timer to update all snakes.
	Timer updateTimer;

	/// Timer to add another snake.
	Timer addTimer;

	void addSnake();
};

#endif //ANIMATIONSNAKES_H_DEFINED

