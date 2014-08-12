#ifndef LAYOUT_H_DEFINED
#define LAYOUT_H_DEFINED

#include <Engine/Graphics/Window.hpp>

/// Interface for how the things are shown on the screen.
///
/// Any Layout will have lots if Windows, each with a specific
/// function: to show the game board, display high scores,
/// show next pieces, and such.
///
/// I suggest you subclass this and implement your layout
/// elsewhere.
///
class Layout
{
public:
	/// Full width of the terminal right now.
	static int screenWidth;

	/// Full height of the terminal right now.
	static int screenHeight;

	Layout(int width, int height);
	virtual ~Layout();

	virtual void windowsInit();
	virtual void windowsExit();

	virtual void draw();

	/// Layout's main Window, where all the others are inside.
	Window* main;
};

#endif //LAYOUT_H_DEFINED

