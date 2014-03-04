#ifndef LAYOUTGAMEMODESURVIVAL_H_DEFINED
#define LAYOUTGAMEMODESURVIVAL_H_DEFINED

#include <Interface/Layout.hpp>
#include <Interface/Menu/Menu.hpp>
#include <Interface/WindowGameHelp.hpp>
#include <Game/Game.hpp>

#include <vector>

class LayoutGame: public Layout
{
public:
	LayoutGame(Game* game, int width, int height);
	virtual ~LayoutGame();

	void windowsInit();
	void windowsExit();

	void draw(Menu* menu);

private:
	Game* game;

	// On `Layout` we have a `main` Window, where
	// everything's inside

	/// Where the game is shown.
	Window* gamewin;

	/// Where we display game stats (points, etc)
	Window* info;

// HACK so we can get width and height to create menu
//      on the Game
public:
	/// Contains the pause menu.
	Window* pause;

	/// Contains the help screen.
	Window* help;

private:
	Window* boardwin;

	WindowGameHelp* helpWindows;
};

#endif //LAYOUTGAMEMODESURVIVAL_H_DEFINED

