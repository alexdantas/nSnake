#ifndef GAMESTATEMAINMENU_H_DEFINED
#define GAMESTATEMAINMENU_H_DEFINED

#include <Engine/Flow/GameState.hpp>
#include <Display/Layouts/LayoutMainMenu.hpp>
#include <Engine/Graphics/Widgets/Menu.hpp>
#include <Engine/Graphics/Widgets/Menu/MenuAlphabetic.hpp>
#include <Display/WindowGameHelp.hpp>

/// The Main Menu.
///
class GameStateMainMenu: public GameState
{
	friend class LayoutMainMenu;

public:
	GameStateMainMenu();
	virtual ~GameStateMainMenu() { };

    void load();
    void unload();

	void update();
    void draw();

private:
	LayoutMainMenu* layout;

	/// The main menu.
	Menu* menu;

	MenuAlphabetic* menuLevels;
	bool menuLevelsActivated;

	Menu* menuGameSettings;
	bool menuGameSettingsActivated;

	Menu* menuGUIOptions;
 	bool menuGUIOptionsActivated;

	Menu* menuControls;
	bool menuControlsActivated;

	WindowGameHelp* helpWindows;

	// easily create internal menus
	void createMainMenu();
	void createGameSettingsMenu();
	void createLevelsMenu();
	void createGUIOptionsMenu();
	void createControlsMenu();
	void saveSettingsMenuGUIOptions();
	void saveSettingsMenuGameSettings();
};

#endif //GAMESTATEMAINMENU_H_DEFINED

