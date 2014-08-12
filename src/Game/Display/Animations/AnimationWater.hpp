#ifndef ANIMATIONWATER_H_DEFINED
#define ANIMATIONWATER_H_DEFINED

#include <Engine/Graphics/Animation.hpp>
#include <Engine/Helpers/Array2D.hpp>
#include <Engine/Helpers/Timer.hpp>

#include <vector>

// Height of the Water
#define HEIGHT_MIN 0
#define HEIGHT_MAX 100

/// Returns #x percent of the whole intensity set of values.
#define HEIGHT_PERCENT(x) \
	(x * (100/(HEIGHT_MAX - HEIGHT_MIN)))

/// Rules and behavior of the Water animation.
class AnimationWater: public Animation
{
public:
	AnimationWater(Window* window);
	virtual ~AnimationWater();

	void load();
	void update();
	void draw();

private:
	Array2D<int>* buffer1;
	Array2D<int>* buffer2;

	Timer timer;
};

#endif //ANIMATIONWATER_H_DEFINED

