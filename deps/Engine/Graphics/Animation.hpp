#ifndef ANIMATION_H_DEFINED
#define ANIMATION_H_DEFINED

#include <Engine/Graphics/Window.hpp>

/// Abstract interface to any kind of Animation.
class Animation
{
public:
	/// Creates an Animation that will occur on #window.
	Animation(Window* window):
		window(window)
	{ };

	virtual ~Animation() {};

	/// Loads all internal things.
	virtual void load() = 0;

	/// Updates Animation's internal state.
	virtual void update() = 0;

	/// Shows Animation on the screen.
	virtual void draw() = 0;

protected:
	Window* window;
};

#endif //ANIMATION_H_DEFINED

