#include <Flow/GameStateMainMenu.hpp>
#include <Interface/Ncurses.hpp>
#include <Interface/Dialog.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	// Main Menu
	ARCADE,
	OPTIONS,
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
	ERASE_HIGH_SCORES
};

GameStateMainMenu::GameStateMainMenu():
	layout(nullptr),
	menu(nullptr),
	menuArcade(nullptr),
	menuArcadeActivated(false),
	menuOptions(nullptr),
	menuOptionsActvated(false)
{ }
GameStateMainMenu::~GameStateMainMenu()
{ }
void GameStateMainMenu::load(int stack)
{
	UNUSED(stack);

	this->layout = new LayoutMainMenu(80, 24, this);

	createMainMenu();
	createArcadeMenu();
	createOptionsMenu();
}

int GameStateMainMenu::unload()
{
	saveSettingsMenuArcade();
	saveSettingsMenuOptions();

	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->menuArcade);
	SAFE_DELETE(this->menu);

	return 0;
}

GameState::StateCode GameStateMainMenu::update()
{
	int input = Ncurses::getInput(100);

	if (input == 'q')
		return GameState::QUIT;

	if (this->menuArcadeActivated)
	{
		this->menuArcade->handleInput(input);

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
	else if (this->menuOptionsActvated)
	{
		this->menuOptions->handleInput(input);

		if (this->menuOptions->willQuit())
		{
			switch(this->menuOptions->currentID())
			{
			case GO_BACK:
				this->menuOptionsActvated = false;

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
	else
	{
		// We're still at the Main Menu
		this->menu->handleInput(input);

		if (this->menu->willQuit())
		{
			switch(this->menu->currentID())
			{
			case ARCADE:
				this->menuArcadeActivated = true;
				break;

			case OPTIONS:
				this->menuOptionsActvated = true;
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

	else if (this->menuOptionsActvated)
		this->layout->draw(this->menuOptions);

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

	item = new MenuItem("Options", OPTIONS);
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

	number = new MenuItemNumberbox("Starting Level", STARTING_LEVEL, 1, 22, Globals::Game::starting_level);
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

	item = new MenuItem("Erase High Scores",
	                    ERASE_HIGH_SCORES);
	menuOptions->add(item);
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


