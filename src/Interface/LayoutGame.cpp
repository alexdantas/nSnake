#include <Interface/LayoutGame.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

LayoutGame::LayoutGame(Game* game, int width, int height):
	Layout(width, height),
	game(game),
	gamewin(nullptr),
	info(nullptr),
	pause(nullptr),
	help(nullptr)
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

	// We'll start all the windows inside the Layout
	this->main->clear();

	// Leftmost window
	this->gamewin = new Window(this->main,
	                           WINDOW_FILL,
	                           WINDOW_FILL,
	                           WINDOW_FILL,
	                           this->main->getH() - 2);
	this->gamewin->setTitle("nsnake");

	this->info = new Window(this->main,
	                        WINDOW_FILL,
	                        this->main->getH() - 1,
	                        WINDOW_FILL,
	                        1);

	// Le pause window.
	this->pause = new Window(this->main,
	                         this->main->getW() / 4,
	                         this->main->getH() / 2 - 1, //center
	                         this->main->getW() / 2,
	                         6);

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
}
void LayoutGame::windowsExit()
{
	SAFE_DELETE(this->gamewin);
	SAFE_DELETE(this->info);
	SAFE_DELETE(this->help);
	SAFE_DELETE(this->pause);

	this->main->clear(); // clear() as in Window
	this->main->refresh(); // clear() as in Window

	Layout::windowsExit();
}
void LayoutGame::draw(Menu* menu)
{
	if (! this->game)
		return;

	// Will only show the requested windows then exit.
	if (this->game->isPaused)
	{
		if (this->game->showPauseMenu)
		{
			this->pause->clear();
			menu->draw(this->pause);
			this->pause->refresh();
		}
		else if (this->game->showHelp)
		{
			this->help->clear();
			this->help->print("Game keys",
			                  this->help->getW()/2 - 9/2, // center
			                  1,
			                  Globals::Theme::hilite_text);

			this->help->print_multiline("Arrow keys     Move snake\n"
			                            "q              Quit\n"
			                            "p              Pause/Unpause\n"
			                            "h              Show help",
			                            1,
			                            3,
			                            Globals::Theme::text);
			this->help->refresh();
		}

		// NCURSES NEEDS THIS
		refresh();
		return;
	}

	// Statistics
	// A mess of direct Ncurses calls - fix this later
	this->info->clear();

	// ColorPair hilite = Colors::pair(COLOR_BLUE, COLOR_DEFAULT, true);

	// int lowest_y = this->score->getH() - 2; // border

	// this->score->print("Hi-Score", 1, lowest_y - 7, hilite);
	// this->score->print("Score",    1, lowest_y - 4, hilite);
	// this->score->print("Level",    1, lowest_y - 1, hilite);

	// // Default color
	// wattrset(this->score->win, COLOR_PAIR(0));

	// mvwprintw(this->score->win, lowest_y - 6, 1, "%8u", 9000);
	// mvwprintw(this->score->win, lowest_y - 3, 1, "%8u", this->game->score->points);
	// mvwprintw(this->score->win, lowest_y    , 1, "%8d", this->game->score->level);

	// this->score->refresh();

	// / /// // // / / // / /

	// 	this->rightmost->clear();

	// 	// User name
	// 	this->rightmost->print(this->game->score->name, this->rightmost->getW() - this->game->score->name.size() - 2, 01, Colors::pair(COLOR_BLUE, COLOR_DEFAULT));

	// 	// Piece and Line Statistics
	// 	this->rightmost->print("[I]", 2, 2, Globals::Theme::piece_I->color);
	// 	this->rightmost->print("[T]", 2, 3, Globals::Theme::piece_T->color);
	// 	this->rightmost->print("[L]", 2, 4, Globals::Theme::piece_L->color);
	// 	this->rightmost->print("[J]", 2, 5, Globals::Theme::piece_J->color);
	// 	this->rightmost->print("[S]", 2, 6, Globals::Theme::piece_S->color);
	// 	this->rightmost->print("[Z]", 2, 7, Globals::Theme::piece_Z->color);
	// 	this->rightmost->print("[O]", 2, 8, Globals::Theme::piece_O->color);

	// 	int x_offset = this->rightmost->getW() - 15;

	// 	this->rightmost->print("Singles", x_offset, 2, hilite);
	// 	this->rightmost->print("Doubles", x_offset, 3, hilite);
	// 	this->rightmost->print("Triples", x_offset, 4, hilite);
	// 	this->rightmost->print("Tetris",  x_offset, 5, hilite);

	// 	this->rightmost->print("Pieces", x_offset, 7, hilite);
	// 	this->rightmost->print("Lines",  x_offset, 8, hilite);

	// 	wattrset(this->rightmost->win, COLOR_PAIR(0));
	// 	mvwprintw(this->rightmost->win, 2, 6, "x %3d", this->game->stats.I);
	// 	mvwprintw(this->rightmost->win, 3, 6, "x %3d", this->game->stats.T);
	// 	mvwprintw(this->rightmost->win, 4, 6, "x %3d", this->game->stats.L);
	// 	mvwprintw(this->rightmost->win, 5, 6, "x %3d", this->game->stats.J);
	// 	mvwprintw(this->rightmost->win, 6, 6, "x %3d", this->game->stats.S);
	// 	mvwprintw(this->rightmost->win, 7, 6, "x %3d", this->game->stats.Z);
	// 	mvwprintw(this->rightmost->win, 8, 6, "x %3d", this->game->stats.O);

	// 	mvwprintw(this->rightmost->win, 2, x_offset + 8, "x %3d", this->game->stats.singles);
	// 	mvwprintw(this->rightmost->win, 3, x_offset + 8, "x %3d", this->game->stats.doubles);
	// 	mvwprintw(this->rightmost->win, 4, x_offset + 8, "x %3d", this->game->stats.triples);
	// 	mvwprintw(this->rightmost->win, 5, x_offset + 8, "x %3d", this->game->stats.tetris);

	// 	mvwprintw(this->rightmost->win, 7, x_offset + 7, "x %4d", this->game->stats.pieces);
	// 	mvwprintw(this->rightmost->win, 8, x_offset + 7, "x %4d", this->game->stats.lines);

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

	this->game->player->draw(this->gamewin);

	this->gamewin->refresh();

	// NCURSES NEEDS THIS
	refresh();
}

