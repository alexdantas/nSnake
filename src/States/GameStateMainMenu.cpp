#include <States/GameStateMainMenu.hpp>
#include <Engine/InputManager.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Engine/Graphics/Widgets/Dialog.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Config/Globals.hpp>
#include <Entities/BoardParser.hpp>
#include <Entities/ScoreFile.hpp>

#include <Engine/Flow/StateManager.hpp>
#include <States/GameStateGame.hpp>

enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	// Main Menu
	ARCADE=1337,
	LEVELS,
	GAME_SETTINGS,
	HELP,
	GUI_OPTIONS,
	CONTROLS,
	QUIT_GAME,

	// Level select Submenu
	RANDOM,

	// Game Settings Submenu
	GO_BACK,
	STARTING_SPEED,
	TELEPORT,
	FRUITS,
	RANDOM_WALLS,
	BOARD_SIZE,
	SCROLL_DELAY,
	SCROLL_RIGHT,
	SCROLL_LEFT,
	SCROLL_UP,
	SCROLL_DOWN,
	ERASE_HIGH_SCORES,

	// GUI Submenu
	SHOW_BORDERS,
	FANCY_BORDERS,
	OUTER_BORDER,
	USE_COLORS,
	CENTER_HORIZONTAL,
	CENTER_VERTICAL,

	// Controls Submenu
	CONTROLS_KEY_LEFT,
	CONTROLS_KEY_RIGHT,
	CONTROLS_KEY_UP,
	CONTROLS_KEY_DOWN,
	CONTROLS_KEY_PAUSE,
	CONTROLS_KEY_HELP,
	CONTROLS_KEY_QUIT,
	CONTROLS_DEFAULT
};

GameStateMainMenu::GameStateMainMenu():
	layout(NULL),
	menu(NULL),
	menuLevels(NULL),
	menuLevelsActivated(false),
	menuGameSettings(NULL),
	menuGameSettingsActivated(false),
	menuGUIOptions(NULL),
	menuGUIOptionsActivated(false),
	menuControls(NULL),
	menuControlsActivated(false),
	helpWindows(NULL)
{ }

void GameStateMainMenu::load()
{
	this->layout = new LayoutMainMenu(80, 24, this);

	createMainMenu();
	createLevelsMenu();
	createGameSettingsMenu();
	createGUIOptionsMenu();
	createControlsMenu();

	this->helpWindows = new WindowGameHelp();
}

void GameStateMainMenu::unload()
{
	saveSettingsMenuGameSettings();
	saveSettingsMenuGUIOptions();

	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->menuLevels);
	SAFE_DELETE(this->menuGameSettings);
	SAFE_DELETE(this->menuGUIOptions);
	SAFE_DELETE(this->menuControls);
	SAFE_DELETE(this->menu);
}

void GameStateMainMenu::update()
{
	if (InputManager::isPressed("quit"))
		StateManager::quit();

	if (this->menuLevelsActivated)
	{
		this->menuLevels->handleInput();

		if (this->menuLevels->willQuit())
		{
			switch (this->menuLevels->currentID())
			{
			case GO_BACK:
				this->layout->menu->setTitle("Main Menu");
				this->menuLevelsActivated = false;
				break;

			case RANDOM:
			{
				this->menuLevels->goRandom();

				Globals::Game::current_level = this->menuLevels->current->label;
				StateManager::change(new GameStateGame());
				break;
			}

			default:
				// Selected a level name!
				Globals::Game::current_level = this->menuLevels->current->label;
				StateManager::change(new GameStateGame());
				break;
			}
			this->menuLevels->reset();
		}
	}
	else if (this->menuGameSettingsActivated)
	{
		this->menuGameSettings->handleInput();

		if (this->menuGameSettings->willQuit())
		{
			saveSettingsMenuGameSettings();

			// And then exit based on the selected option.
			switch (this->menuGameSettings->currentID())
			{
			case ERASE_HIGH_SCORES:
			{
				bool answer = Dialog::askBool("Are you sure?");

				if (answer)
				{
					ScoreFile::eraseAll();
					Dialog::show("All high scores erased!", true);
				}
			}
				break;

			case GO_BACK:
				this->layout->menu->setTitle("Main Menu");
				this->menuGameSettingsActivated = false;
				break;
			}
			this->menuGameSettings->reset();
		}
	}
	else if (this->menuGUIOptionsActivated)
	{
		this->menuGUIOptions->handleInput();

		if (this->menuGUIOptions->willQuit())
		{
			switch(this->menuGUIOptions->currentID())
			{
			case GO_BACK:
				this->layout->menu->setTitle("Main Menu");
				this->menuGUIOptionsActivated = false;

				// Redrawing the screen to refresh settings
				saveSettingsMenuGUIOptions();
				this->layout->windowsExit();
				this->layout->windowsInit();
				break;
			}
			this->menuGUIOptions->reset();
		}
	}
	else if (this->menuControlsActivated)
	{
		this->menuControls->handleInput();

		if (this->menuControls->willQuit())
		{
			std::string key(""); // for key binding

			switch(this->menuControls->currentID())
			{
			case GO_BACK:
				this->layout->menu->setTitle("Main Menu");
				this->menuControlsActivated = false;
				break;

			case CONTROLS_KEY_LEFT:  key = "left";  break;
			case CONTROLS_KEY_RIGHT: key = "right"; break;
			case CONTROLS_KEY_UP:    key = "up";    break;
			case CONTROLS_KEY_DOWN:  key = "down";  break;
			case CONTROLS_KEY_PAUSE: key = "pause"; break;
			case CONTROLS_KEY_HELP:  key = "help"; break;
			case CONTROLS_KEY_QUIT:  key = "quit";  break;

			case CONTROLS_DEFAULT:
			{
				// Reset all keybindings to default
				InputManager::bind("left",  KEY_LEFT);
				InputManager::bind("right", KEY_RIGHT);
				InputManager::bind("up",    KEY_UP);
				InputManager::bind("down",  KEY_DOWN);
				InputManager::bind("pause", 'p');
				InputManager::bind("help",  'h');
				InputManager::bind("quit",  'q');

				// Resetting the menu to show the new labels
				createControlsMenu();
				menuControls->goLast();
				break;
			}
			}

			// If we'll change a key binding
			if (! key.empty())
			{
				Dialog::show("Press any key, Enter to Cancel");
				int tmp = Ncurses::getInput(-1);

				if ((tmp != KEY_ENTER) &&
				    (tmp != '\n') &&
				    (tmp != ERR))
				{
					InputManager::bind(key, tmp);

					MenuItemLabel* label;
					label = (MenuItemLabel*)menuControls->current;

					label->set(InputManager::keyToString(tmp));
				}
			}
			this->menuControls->reset();
		}
	}
	else
	{
		// We're still at the Main Menu
		this->menu->handleInput();

		if (this->menu->willQuit())
		{
			switch(this->menu->currentID())
			{
			case ARCADE:
				// Starting game on the default level
				Globals::Game::current_level = "";
				StateManager::change(new GameStateGame());
				break;

			case LEVELS:
				// Before going to the Levels menu, we must check if
				// the user has any levels on the level directory.
				// If not, we should stay at the main menu.
				if (BoardParser::listLevels().size() == 0)
				{
					Dialog::show("Sorry, it seems you have no levels.\n"
					             "\n"
					             "Please copy the default level files from\n"
					             "`"  SYSTEM_LEVEL_DIR "/`\n"
					             "to\n"
					             "`" + BoardParser::directory + "`\n" +
					             "\n"
					             "You can also download more levels from the website:\n"
					             "http://nsnake.alexdantas.net/", true);
				}
				else
				{
					this->layout->menu->setTitle("Level Select");
					this->menuLevelsActivated = true;
				}
				break;

			case GAME_SETTINGS:
				this->layout->menu->setTitle("Game Settings");
				this->menuGameSettingsActivated = true;
				break;

			case GUI_OPTIONS:
				this->layout->menu->setTitle("GUI Options");
				this->menuGUIOptionsActivated = true;
				break;

			case CONTROLS:
				this->layout->menu->setTitle("Controls");
				this->menuControlsActivated = true;
				break;

			case HELP:
				this->helpWindows->run();
				break;

			case QUIT_GAME:
				StateManager::quit();
				break;
			}
			this->menu->reset();
		}
	}
}

void GameStateMainMenu::draw()
{
	if (this->menuLevelsActivated)
		this->layout->draw(this->menuLevels);

	else if (this->menuGameSettingsActivated)
		this->layout->draw(this->menuGameSettings);

	else if (this->menuGUIOptionsActivated)
		this->layout->draw(this->menuGUIOptions);

	else if (this->menuControlsActivated)
		this->layout->draw(this->menuControls);

	else
		this->layout->draw(this->menu);
}

void GameStateMainMenu::createMainMenu()
{
	SAFE_DELETE(this->menu);

	// Creating the Menu and Items.
	// Their default ids will be based on current's
	// settings.
	this->menu = new Menu(1,
	                      1,
	                      this->layout->menu->getW() - 2,
	                      this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Arcade Mode", ARCADE);
	menu->add(item);

	item = new MenuItem("Level Select", LEVELS);
	menu->add(item);

	item = new MenuItem("Game Settings", GAME_SETTINGS);
	menu->add(item);

	item = new MenuItem("GUI Options", GUI_OPTIONS);
	menu->add(item);

	item = new MenuItem("Controls", CONTROLS);
	menu->add(item);

	item = new MenuItem("Help", HELP);
	menu->add(item);

	item = new MenuItem("Quit", QUIT_GAME);
	menu->add(item);
}
void GameStateMainMenu::createLevelsMenu()
{
	SAFE_DELETE(this->menuLevels);

	this->menuLevels = new MenuAlphabetic(1,
	                                      1,
	                                      this->layout->menu->getW() - 2,
	                                      this->layout->menu->getH() - 2);

	MenuItem* item;

	std::vector<std::string> levels = BoardParser::listLevels();

	item = new MenuItem("Back", GO_BACK);
	menuLevels->add(item);

	item = new MenuItem("Random", RANDOM);
	menuLevels->add(item);

	menuLevels->addBlank();

	for (size_t i = 0; i < levels.size(); i++)
	{
		item = new MenuItem(levels[i], i);
		menuLevels->add(item);
	}
}
void GameStateMainMenu::createGameSettingsMenu()
{
	SAFE_DELETE(this->menuGameSettings);

	this->menuGameSettings = new Menu(1,
	                                  1,
	                                  this->layout->menu->getW() - 2,
	                                  this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Back", GO_BACK);
	menuGameSettings->add(item);

	menuGameSettings->addBlank();

	MenuItemNumberbox* number;

	number = new MenuItemNumberbox("Starting Speed", STARTING_SPEED, 1, 10, Globals::Game::starting_speed);
	menuGameSettings->add(number);

	number = new MenuItemNumberbox("Fruits", FRUITS, 1, 99, Globals::Game::fruits_at_once);
	menuGameSettings->add(number);

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Teleport", TELEPORT, Globals::Game::teleport);
	menuGameSettings->add(check);

	check = new MenuItemCheckbox("Random Walls", RANDOM_WALLS, Globals::Game::random_walls);
	menuGameSettings->add(check);

	// The board size
	std::vector<std::string> options;
	options.push_back("Small");
	options.push_back("Medium");
	options.push_back("Large");

	MenuItemTextlist* list;

	// the default board size
	std::string defaullt;

	switch (Globals::Game::board_size)
	{
	case Globals::Game::SMALL:  defaullt = "Small";  break;
	case Globals::Game::MEDIUM: defaullt = "Medium"; break;
	default:                    defaullt = "Large";  break;
	}

	list = new MenuItemTextlist("Maze size",
	                            BOARD_SIZE,
	                            options,
	                            defaullt);

	menuGameSettings->add(list);

	menuGameSettings->addBlank();

	number = new MenuItemNumberbox("Scroll Delay(ms)", SCROLL_DELAY, 100, 5000, Globals::Game::board_scroll_delay, 100);
	menuGameSettings->add(number);

	check = new MenuItemCheckbox("Scroll Up", SCROLL_UP, Globals::Game::board_scroll_up);
	menuGameSettings->add(check);

	check = new MenuItemCheckbox("Scroll Down", SCROLL_DOWN, Globals::Game::board_scroll_down);
	menuGameSettings->add(check);

	check = new MenuItemCheckbox("Scroll Left", SCROLL_LEFT, Globals::Game::board_scroll_left);
	menuGameSettings->add(check);

	check = new MenuItemCheckbox("Scroll Right", SCROLL_RIGHT, Globals::Game::board_scroll_right);
	menuGameSettings->add(check);

	menuGameSettings->addBlank();

	item = new MenuItem("Erase High Scores",
	                    ERASE_HIGH_SCORES);
	menuGameSettings->add(item);
}
void GameStateMainMenu::createGUIOptionsMenu()
{
	SAFE_DELETE(this->menuGUIOptions);

	this->menuGUIOptions = new Menu(1,
	                             1,
	                             this->layout->menu->getW() - 2,
	                             this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Back", GO_BACK);
	menuGUIOptions->add(item);

	menuGUIOptions->addBlank();

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Show Borders",
	                             SHOW_BORDERS,
	                             EngineGlobals::Screen::show_borders);
	menuGUIOptions->add(check);

	check = new MenuItemCheckbox("Fancy Borders",
	                             FANCY_BORDERS,
	                             EngineGlobals::Screen::fancy_borders);
	menuGUIOptions->add(check);

	check = new MenuItemCheckbox("Outer Border",
	                             OUTER_BORDER,
	                             EngineGlobals::Screen::outer_border);
	menuGUIOptions->add(check);

	check = new MenuItemCheckbox("Center Horizontal",
	                             CENTER_HORIZONTAL,
	                             EngineGlobals::Screen::center_horizontally);
	menuGUIOptions->add(check);

	check = new MenuItemCheckbox("Center Vertical",
	                             CENTER_VERTICAL,
	                             EngineGlobals::Screen::center_vertically);
	menuGUIOptions->add(check);
}
void GameStateMainMenu::createControlsMenu()
{
	SAFE_DELETE(this->menuControls);

	this->menuControls = new Menu(1,
	                              1,
	                              this->layout->menu->getW() - 2,
	                              this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Back", GO_BACK);
	menuControls->add(item);

	menuControls->addBlank();

	MenuItemLabel* label;
	std::string str;

	str = InputManager::keyToString(InputManager::getBind("up"));
	label = new MenuItemLabel("Key up", CONTROLS_KEY_UP, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("down"));
	label = new MenuItemLabel("Key down", CONTROLS_KEY_DOWN, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("left"));
	label = new MenuItemLabel("Key left", CONTROLS_KEY_LEFT, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("right"));
	label = new MenuItemLabel("Key right", CONTROLS_KEY_RIGHT, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("pause"));
	label = new MenuItemLabel("Key pause", CONTROLS_KEY_PAUSE, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("help"));
	label = new MenuItemLabel("Key help", CONTROLS_KEY_HELP, str);
	menuControls->add(label);

	str = InputManager::keyToString(InputManager::getBind("quit"));
	label = new MenuItemLabel("Key quit", CONTROLS_KEY_QUIT, str);
	menuControls->add(label);

	menuControls->addBlank();

	item = new MenuItem("Reset to Defaults", CONTROLS_DEFAULT);
	menuControls->add(item);
}
void GameStateMainMenu::saveSettingsMenuGUIOptions()
{
	if (!this->menuGUIOptions)
		return;

	// User selected an option
	// Let's get ids from menu items
	EngineGlobals::Screen::show_borders        = this->menuGUIOptions->getBool(SHOW_BORDERS);
	EngineGlobals::Screen::fancy_borders       = this->menuGUIOptions->getBool(FANCY_BORDERS);
	EngineGlobals::Screen::outer_border        = this->menuGUIOptions->getBool(OUTER_BORDER);
	EngineGlobals::Screen::center_horizontally = this->menuGUIOptions->getBool(CENTER_HORIZONTAL);
	EngineGlobals::Screen::center_vertically   = this->menuGUIOptions->getBool(CENTER_VERTICAL);
}
void GameStateMainMenu::saveSettingsMenuGameSettings()
{
	if (!this->menuGameSettings)
		return;

	// User selected an option
	// Let's get ids from menu items
	Globals::Game::starting_speed = (unsigned int)this->menuGameSettings->getInt(STARTING_SPEED);
	Globals::Game::fruits_at_once = this->menuGameSettings->getInt(FRUITS);
	Globals::Game::random_walls = this->menuGameSettings->getBool(RANDOM_WALLS);
	Globals::Game::teleport = this->menuGameSettings->getBool(TELEPORT);

	std::string tmp = this->menuGameSettings->getString(BOARD_SIZE);
	if (tmp == "Small")
		Globals::Game::board_size = Globals::Game::SMALL;

	else if (tmp == "Medium")
		Globals::Game::board_size = Globals::Game::MEDIUM;

	else
		Globals::Game::board_size = Globals::Game::LARGE;

	Globals::Game::board_scroll_delay = this->menuGameSettings->getInt(SCROLL_DELAY);

	Globals::Game::board_scroll_left  = this->menuGameSettings->getBool(SCROLL_LEFT);
	Globals::Game::board_scroll_right = this->menuGameSettings->getBool(SCROLL_RIGHT);
	Globals::Game::board_scroll_up    = this->menuGameSettings->getBool(SCROLL_UP);
	Globals::Game::board_scroll_down  = this->menuGameSettings->getBool(SCROLL_DOWN);
}
