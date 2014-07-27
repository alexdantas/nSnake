#include <Interface/LayoutGame.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

LayoutGame::LayoutGame(Game* game, int width, int height):
	Layout(width, height),
	game(game),
	gamewin(NULL),
	info(NULL),
	pause(NULL),
	help(NULL),
	boardwin(NULL),
	helpWindows(NULL)
{
	this->windowsInit();
}
LayoutGame::~LayoutGame()
{
	this->windowsExit();
}
void LayoutGame::windowsInit()
{
	Layout::windowsInit();
	this->main->setTitle("nsnake " VERSION);

	if (this->game->currentScore->level.empty())
		this->main->setTitle("Arcade Mode", Window::TOP_RIGHT);
	else
		this->main->setTitle("Level " + this->game->board->getMetadata("name"), Window::TOP_RIGHT);

	// Leftmost window
	this->gamewin = new Window(this->main,
	                           WINDOW_FILL,
	                           WINDOW_FILL,
	                           WINDOW_FILL,
	                           this->main->getH() - 3);

	this->info = new Window(this->main,
	                        WINDOW_FILL,
	                        this->main->getH() - 2,
	                        WINDOW_FILL,
	                        1);

	// Le pause window.
	this->pause = new Window(this->main,
	                         this->main->getW() / 4,
	                         this->main->getH() / 2 - 1, //center
	                         this->main->getW() / 2,
	                         7);

	if (Globals::Screen::show_borders)
	{
		this->pause->borders(Globals::Screen::fancy_borders ?
		                     Window::BORDER_FANCY :
		                     Window::BORDER_REGULAR);
	}
	this->pause->setTitle("Paused");

	// Le help window.
	this->help = new Window(this->main,
	                        this->main->getW() / 4,
	                        this->main->getH() / 4,
	                        this->main->getW() / 2,
	                        this->main->getH() / 2);

	if (Globals::Screen::show_borders)
	{
		this->help->borders(Globals::Screen::fancy_borders ?
		                     Window::BORDER_FANCY :
		                     Window::BORDER_REGULAR);
	}
	this->help->setTitle("Help");

	this->helpWindows = new WindowGameHelp();
}
void LayoutGame::windowsExit()
{
	SAFE_DELETE(this->gamewin);
	SAFE_DELETE(this->info);
	SAFE_DELETE(this->pause);
	SAFE_DELETE(this->help);
	SAFE_DELETE(this->boardwin);
	SAFE_DELETE(this->helpWindows);

	this->main->clear(); // clear() as in Window
	this->main->refresh(); // clear() as in Window

	Layout::windowsExit();
}
void LayoutGame::draw(Menu* menu)
{
	if (! this->game)
		return;

	// This hack allows the game board to be centered
	if (! this->boardwin)
	{
		// initializing for the first time
		int x = this->gamewin->getW()/2 - this->game->board->getW()/2;
		int y = this->gamewin->getH()/2 - this->game->board->getH()/2;
		int w = this->game->board->getW();
		int h = this->game->board->getH();

		boardwin = new Window(this->gamewin, x, y, w, h);
	}

	this->main->clear();

	// Will only show the requested windows then exit.

	if (this->game->isPaused && this->game->showPauseMenu)
	{
		this->pause->clear();
		menu->draw(this->pause);
		this->pause->refresh();

		refresh();
		return;
	}

	if (this->game->showHelp)
	{
		this->helpWindows->run();
		this->game->showHelp = false;

		// NCURSES NEEDS THIS
		refresh();
		return;
	}

	// Statistics
	// A mess of direct Ncurses calls - fix this later
	this->info->clear();
	this->info->print("a", 0, 0, 0);

	ColorPair hilite = Globals::Theme::hilite_text;

	this->info->print("Hi-Score", 0, 0, hilite);
	this->info->print("Score",    this->info->getW()/3, 0, hilite);
	this->info->print("Speed",    this->info->getW()/3 * 2, 0, hilite);

	// Default color
	wattrset(this->info->win, COLOR_PAIR(0));

	if (this->game->scores->highScore)
		mvwprintw(this->info->win, 0, 9, "%u", this->game->scores->highScore->points);

	mvwprintw(this->info->win, 0, this->info->getW()/3 + 6, "%u", this->game->currentScore->points);
	mvwprintw(this->info->win, 0, this->info->getW()/3 * 2 + 6, "%d", this->game->currentScore->speed);

	// 	// Timer - how much time has passed since game start
	// 	this->rightmost->print("Timer", 2, 10, hilite);

	// 	long delta_s = this->game->timer.delta_s();

	// 	int seconds = delta_s % 60;
	// 	int minutes = (delta_s / 60) % 60;
	// 	int hours   = ((delta_s / 60) / 60) % 24;

	// 	wattrset(this->rightmost->win, COLOR_PAIR(0));

	// 	mvwprintw(this->rightmost->win, 10, 8, "%02d:%02d:%02d", hours, minutes, seconds);

	// 	// Delay
	// 	this->rightmost->print("Delay", 2, 12, hilite);
	// 	wattrset(this->rightmost->win, COLOR_PAIR(0));

	// 	mvwprintw(this->rightmost->win, 12, 8, "%dms", this->game->getDelay(this->game->score->level));

	// 	// Bottom line - version and Help
	// 	this->rightmost->print("nsnake v" VERSION, 1, this->rightmost->getH() - 2, Colors::pair(COLOR_CYAN, COLOR_DEFAULT));

	// 	this->rightmost->print("H", this->rightmost->getW() - 5, this->rightmost->getH() - 2, Colors::pair(COLOR_YELLOW, COLOR_DEFAULT));
	// 	this->rightmost->print("elp", this->rightmost->getW() - 4, this->rightmost->getH() - 2, Colors::pair(COLOR_CYAN, COLOR_DEFAULT));
	// 	this->rightmost->refresh();
	// }

	// Board and main game stuff
	this->gamewin->clear();

	this->game->board->draw(boardwin);
	this->game->player->draw(boardwin);
	this->game->fruits->draw(boardwin);

	this->gamewin->refresh();
	this->info->refresh();
	this->main->refresh();

	// NCURSES NEEDS THIS
	refresh();
}

