#ifndef LAYOUTMAINMENU_H_DEFINED
#define LAYOUTMAINMENU_H_DEFINED

#include <Interface/Layout.hpp>
#include <Interface/Window.hpp>
#include <Interface/Menu/Menu.hpp>
#include <Interface/Animation/Animation.hpp>

// circular dependence on GameStateMainMenu
class GameStateMainMenu;

/// How we show the screen at GameStateMainMenu.
///
/// This class is merely a wrapper over functions that
/// print GameStateMainMenu on the screen.
///
/// Thus, it needs full access to it's internal values.
/// So it comes down to this circular dependency.
///
class LayoutMainMenu: public Layout
{
public:
	LayoutMainMenu(int width, int height, GameStateMainMenu* state);
	virtual ~LayoutMainMenu();

	void windowsInit();
	void windowsExit();

	/// Shows the Main Menu screen, along with drawing #menu.
	void draw(Menu* menu);

	/// Redraws everything from scratch, refreshing borders
	/// and stuff.
	void redraw();

	/// We need this so we can access the states' flags
	/// and decide best on how to print it.
	GameStateMainMenu* state;

	Window* logo;
	Window* menu;
	Window* animationwin;

	/// Cure thing at the main menu.
	Animation* animation;
};

#endif //LAYOUTMAINMENU_H_DEFINED

