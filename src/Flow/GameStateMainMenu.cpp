#include <Flow/GameStateMainMenu.hpp>
#include <Flow/InputManager.hpp>
#include <Interface/Ncurses.hpp>
#include <Interface/Dialog.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	// Main Menu
	ARCADE,
	HELP,
	OPTIONS,
	CONTROLS,
	QUIT_GAME,

	// Single Player Submenu
	START_GAME,
	GO_BACK,
	STARTING_LEVEL,
	TELEPORT,
	FRUITS,
	RANDOM_WALLS,
	BOARD_SIZE,

	// Options Submenu
	SHOW_BORDERS,
	FANCY_BORDERS,
	OUTER_BORDER,
	USE_COLORS,
	CENTER_HORIZONTAL,
	CENTER_VERTICAL,
	ERASE_HIGH_SCORES,

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
	menuArcade(NULL),
	menuArcadeActivated(false),
	menuOptions(NULL),
	menuOptionsActivated(false),
	menuControls(NULL),
	menuControlsActivated(false),
	helpWindows(NULL)
{ }

void GameStateMainMenu::load(int stack)
{
	UNUSED(stack);

	this->layout = new LayoutMainMenu(80, 24, this);

	createMainMenu();
	createArcadeMenu();
	createOptionsMenu();
	createControlsMenu();

	this->helpWindows = new WindowGameHelp();
}

int GameStateMainMenu::unload()
{
	saveSettingsMenuArcade();
	saveSettingsMenuOptions();

	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->menuControls);
	SAFE_DELETE(this->menuOptions);
	SAFE_DELETE(this->menuArcade);
	SAFE_DELETE(this->menu);

	return 0;
}

GameState::StateCode GameStateMainMenu::update()
{
	if (InputManager::isPressed("quit"))
		return GameState::QUIT;

	if (this->menuArcadeActivated)
	{
		this->menuArcade->handleInput();

		if (this->menuArcade->willQuit())
		{
			saveSettingsMenuArcade();

			// And then exit based on the selected option.
			switch (this->menuArcade->currentID())
			{
			case START_GAME:
				return GameState::GAME_START;
				break;

			case GO_BACK:
				this->menuArcadeActivated = false;
				break;
			}
			this->menuArcade->reset();
		}
	}
	else if (this->menuOptionsActivated)
	{
		this->menuOptions->handleInput();

		if (this->menuOptions->willQuit())
		{
			switch(this->menuOptions->currentID())
			{
			case GO_BACK:
				this->menuOptionsActivated = false;

				// Redrawing the screen to refresh settings
				saveSettingsMenuOptions();
				this->layout->windowsExit();
				this->layout->windowsInit();
				break;

			case ERASE_HIGH_SCORES:
				bool answer = Dialog::askBool("Are you sure?");

				if (answer)
				{
					// Clearing the High Scores file...
					Utils::File::create(Globals::Config::scoresFile);
				}
				break;
			}
			this->menuOptions->reset();
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
				this->menuArcadeActivated = true;
				break;

			case HELP:
				this->helpWindows->run();
				break;

			case OPTIONS:
				this->menuOptionsActivated = true;
				break;

			case CONTROLS:
				this->menuControlsActivated = true;
				break;

			case QUIT_GAME:
				return GameState::QUIT;
				break;
			}
			this->menu->reset();
		}
	}

	// Otherwise, continuing things...
	return GameState::CONTINUE;
}

void GameStateMainMenu::draw()
{
	if (this->menuArcadeActivated)
		this->layout->draw(this->menuArcade);

	else if (this->menuOptionsActivated)
		this->layout->draw(this->menuOptions);

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

	item = new MenuItem("Arcade", ARCADE);
	menu->add(item);

	item = new MenuItem("Help", HELP);
	menu->add(item);

	item = new MenuItem("Options", OPTIONS);
	menu->add(item);

	item = new MenuItem("Controls", CONTROLS);
	menu->add(item);

	item = new MenuItem("Quit", QUIT_GAME);
	menu->add(item);
}
void GameStateMainMenu::createArcadeMenu()
{
	SAFE_DELETE(this->menuArcade);

	this->menuArcade = new Menu(1,
	                            1,
	                            this->layout->menu->getW() - 2,
	                            this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Start Game", START_GAME);
	menuArcade->add(item);

	item = new MenuItem("Back", GO_BACK);
	menuArcade->add(item);

	menuArcade->addBlank();

	MenuItemNumberbox* number;

	number = new MenuItemNumberbox("Starting Level", STARTING_LEVEL, 1, 10, Globals::Game::starting_level);
	menuArcade->add(number);

	number = new MenuItemNumberbox("Fruits", FRUITS, 1, 99, Globals::Game::fruits_at_once);
	menuArcade->add(number);

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Teleport", TELEPORT, Globals::Game::teleport);
	menuArcade->add(check);

	check = new MenuItemCheckbox("Random Walls", RANDOM_WALLS, Globals::Game::random_walls);
	menuArcade->add(check);

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
	menuArcade->add(list);
}
void GameStateMainMenu::createOptionsMenu()
{
	SAFE_DELETE(this->menuOptions);

	this->menuOptions = new Menu(1,
	                             1,
	                             this->layout->menu->getW() - 2,
	                             this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Back", GO_BACK);
	menuOptions->add(item);

	menuOptions->addBlank();

	MenuItemCheckbox* check;

	check = new MenuItemCheckbox("Show Borders",
	                             SHOW_BORDERS,
	                             Globals::Screen::show_borders);
	menuOptions->add(check);

	check = new MenuItemCheckbox("Fancy Borders",
	                             FANCY_BORDERS,
	                             Globals::Screen::fancy_borders);
	menuOptions->add(check);

	check = new MenuItemCheckbox("Outer Border",
	                             OUTER_BORDER,
	                             Globals::Screen::outer_border);
	menuOptions->add(check);

	check = new MenuItemCheckbox("Center Horizontal",
	                             CENTER_HORIZONTAL,
	                             Globals::Screen::center_horizontally);
	menuOptions->add(check);

	check = new MenuItemCheckbox("Center Vertical",
	                             CENTER_VERTICAL,
	                             Globals::Screen::center_vertically);
	menuOptions->add(check);

	menuOptions->addBlank();

	item = new MenuItem("Erase High Scores",
	                    ERASE_HIGH_SCORES);
	menuOptions->add(item);
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
void GameStateMainMenu::saveSettingsMenuOptions()
{
	if (!this->menuOptions)
		return;

	// User selected an option
	// Let's get ids from menu items
	Globals::Screen::show_borders        = this->menuOptions->getBool(SHOW_BORDERS);
	Globals::Screen::fancy_borders       = this->menuOptions->getBool(FANCY_BORDERS);
	Globals::Screen::outer_border        = this->menuOptions->getBool(OUTER_BORDER);
	Globals::Screen::center_horizontally = this->menuOptions->getBool(CENTER_HORIZONTAL);
	Globals::Screen::center_vertically   = this->menuOptions->getBool(CENTER_VERTICAL);
}
void GameStateMainMenu::saveSettingsMenuArcade()
{
	if (!this->menuArcade)
		return;

	// User selected an option
	// Let's get ids from menu items
	Globals::Game::starting_level = (unsigned int)this->menuArcade->getInt(STARTING_LEVEL);
	Globals::Game::fruits_at_once = this->menuArcade->getInt(FRUITS);
	Globals::Game::random_walls = this->menuArcade->getBool(RANDOM_WALLS);
	Globals::Game::teleport = this->menuArcade->getBool(TELEPORT);

	std::string tmp = this->menuArcade->getString(BOARD_SIZE);
	if (tmp == "Small")
		Globals::Game::board_size = Globals::Game::SMALL;

	else if (tmp == "Medium")
		Globals::Game::board_size = Globals::Game::MEDIUM;

	else
		Globals::Game::board_size = Globals::Game::LARGE;
}


