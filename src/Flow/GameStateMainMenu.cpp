#include <Flow/GameStateMainMenu.hpp>
#include <Interface/Ncurses.hpp>
#include <Misc/Utils.hpp>
#include <Config/Globals.hpp>

enum NamesToEasilyIdentifyTheMenuItemsInsteadOfRawNumbers
{
	// Main Menu
	SINGLE_PLAYER,
	OPTIONS,
	QUIT_GAME,

	// Single Player Submenu
	START_GAME,
	GO_BACK,
	STARTING_LEVEL,

	// Options Submenu
	SHOW_BORDERS,
	FANCY_BORDERS,
	OUTER_BORDER,
	USE_COLORS,
	CENTER_HORIZONTAL,
	CENTER_VERTICAL,
	RANDOM_ALGORITHM
};

GameStateMainMenu::GameStateMainMenu():
	layout(nullptr),
	menu(nullptr),
	menuSinglePlayer(nullptr),
	menuSinglePlayerActivated(false),
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
	createSinglePlayerMenu();
	createOptionsMenu();
}

int GameStateMainMenu::unload()
{
	saveSettingsMenuSinglePlayer();
	saveSettingsMenuOptions();

	SAFE_DELETE(this->layout);
	SAFE_DELETE(this->menuSinglePlayer);
	SAFE_DELETE(this->menu);

	return 0;
}

GameState::StateCode GameStateMainMenu::update()
{
	int input = Ncurses::getInput(100);

	if (input == 'q')
		return GameState::QUIT;

	if (this->menuSinglePlayerActivated)
	{
		this->menuSinglePlayer->handleInput(input);

		if (this->menuSinglePlayer->willQuit())
		{
			saveSettingsMenuSinglePlayer();

			// And then exit based on the selected option.
			switch (this->menuSinglePlayer->currentID())
			{
			case START_GAME:
				return GameState::GAME_START;
				break;

			case GO_BACK:
				this->menuSinglePlayerActivated = false;
				break;
			}
			this->menuSinglePlayer->reset();
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
			case SINGLE_PLAYER:
				this->menuSinglePlayerActivated = true;
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
	if (this->menuSinglePlayerActivated)
		this->layout->draw(this->menuSinglePlayer);

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

	item = new MenuItem("Single Player", SINGLE_PLAYER);
	menu->add(item);

	item = new MenuItem("Options", OPTIONS);
	menu->add(item);

	item = new MenuItem("Quit", QUIT_GAME);
	menu->add(item);
}
void GameStateMainMenu::createSinglePlayerMenu()
{
	SAFE_DELETE(this->menuSinglePlayer);

	this->menuSinglePlayer = new Menu(1,
	                                  1,
	                                  this->layout->menu->getW() - 2,
	                                  this->layout->menu->getH() - 2);

	MenuItem* item;

	item = new MenuItem("Start Game", START_GAME);
	menuSinglePlayer->add(item);

	item = new MenuItem("Back", GO_BACK);
	menuSinglePlayer->add(item);

	menuSinglePlayer->addBlank();

	MenuItemNumberbox* number;

	number = new MenuItemNumberbox("Starting Level", STARTING_LEVEL, 1, 22, Globals::Game::starting_level);
	menuSinglePlayer->add(number);

	this->menuOptions = new Menu(1,
	                             1,
	                             this->layout->menu->getW() - 2,
	                             this->layout->menu->getH() - 2);

	item = new MenuItem("Back", GO_BACK);
	menuOptions->add(item);
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

	// std::vector<std::string> options;
	// options.push_back("regular");
	// options.push_back("dumb");

	// MenuItemTextlist* list;

	// list = new MenuItemTextlist("Random Fruits",
	//                             RANDOM_ALGORITHM,
	//                             options,
	//                             Globals::Game::random_algorithm);
	// menuOptions->add(list);
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
	Globals::Game::random_algorithm      = this->menuOptions->getString(RANDOM_ALGORITHM);
}
void GameStateMainMenu::saveSettingsMenuSinglePlayer()
{
	if (!this->menuSinglePlayer)
		return;

	// User selected an option
	// Let's get ids from menu items
	Globals::Game::starting_level = (unsigned int)this->menuSinglePlayer->getInt(STARTING_LEVEL);
}


