#ifndef ANIMATIONGAMEOFLIFE_H_DEFINED
#define ANIMATIONGAMEOFLIFE_H_DEFINED

#include <Engine/Graphics/Animation.hpp>
#include <Engine/Helpers/Array2D.hpp>
#include <Engine/Helpers/Timer.hpp>

#include <vector>

/// Rules and behavior of the GameOfLife animation.
class AnimationGameOfLife: public Animation
{
public:
	AnimationGameOfLife(Window* window);
	virtual ~AnimationGameOfLife();

	void load();
	void update();
	void draw();

private:
	Array2D<bool>* cells;

	Timer timer;
};

#endif //ANIMATIONGAMEOFLIFE_H_DEFINED

