#include <Interface/WindowGameHelp.hpp>
#include <Interface/Layout.hpp>
#include <Interface/Colors.hpp>
#include <Config/Globals.hpp>
#include <Flow/InputManager.hpp>
#include <Misc/Utils.hpp>

WindowGameHelp::WindowGameHelp()
{
	int width  = 40;
	int height = 17;

	int windowx = Layout::screenWidth/2  - width/2;
	int windowy = Layout::screenHeight/2 - height/2;

	this->main = new Window(windowx,
	                        windowy,
	                        width,
	                        height);

	if (Globals::Screen::show_borders)
	{
		this->main->borders(Globals::Screen::fancy_borders ?
		                    Window::BORDER_FANCY :
		                    Window::BORDER_REGULAR);
	}

	Window* win;

	// Help
	win = new Window(this->main, 0, 0, WINDOW_FILL, WINDOW_FILL);
	this->windows.push_back(win);

	win = new Window(this->main, 0, 0, WINDOW_FILL, WINDOW_FILL);
	this->windows.push_back(win);
}
void WindowGameHelp::run()
{
	int activatedIndex = 0;

	while (true)
	{
		// Refreshing Windows
		this->main->clear();
		this->windows[activatedIndex]->clear();

		this->main->print(((activatedIndex == 0) ?
		                   "(Help)" :
		                   " Help "),
		                  2,
		                  0,
		                  ((activatedIndex == 0) ?
		                   Globals::Theme::textbox :
		                   Globals::Theme::hilite_text));

		this->main->print(((activatedIndex == 1) ?
		                   "(Credits)" :
		                   " Credits "),
		                  12,
		                  0,
		                  ((activatedIndex == 1) ?
		                   Globals::Theme::textbox :
		                   Globals::Theme::hilite_text));

		// HACK TO AVOID THE BORDERS FROM BEING
		// BOLD. GOTTA SOLVE THIS MISTERY.
		Colors::pairActivate(this->main->win, Globals::Theme::text);

		// Help Window
		if (activatedIndex == 0)
		{
			this->windows[0]->print("In-game controls:\n",
			                        0, 0,
			                        Globals::Theme::hilite_text);

			this->windows[0]->print(Utils::String::split("Move up\n"
			                                             "Move down\n"
			                                             "Move left\n"
			                                             "Move right\n"
			                                             "Pause game\n"
			                                             "Quit anytime\n"
			                                             "Show help", '\n'),
			                        1, 1,
			                        Globals::Theme::hilite_text);

			this->windows[0]->print(Utils::String::split(InputManager::keyToString(InputManager::getBind("up")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("down")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("left")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("right")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("pause")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("quit")) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind("help")), '\n'),
			                        14, 1,
			                        Globals::Theme::text);

			this->windows[0]->print("Menu controls:\n",
			                        0, 9,
			                        Globals::Theme::hilite_text);

			this->windows[0]->print(Utils::String::split("First item\n"
			                                             "Last item", '\n'),
			                        1, 10,
			                        Globals::Theme::hilite_text);

			this->windows[0]->print(Utils::String::split("page up\n"
			                                             "page down", '\n'),
			                        14, 10,
			                        Globals::Theme::text);

			this->windows[0]->print(Utils::String::split(" Settings and scores are stored at:\n"
			                                             " `~/.local/share/nsnake/`", '\n'),
			                        0, 13,
			                        Globals::Theme::text);
		}
		//
		// Credits
		else if (activatedIndex == 1)
		{
			this->windows[1]->print(Utils::String::split(" _      __   _       __    _     ____ \n"
			                                             "| |\\ | ( (` | |\\ |  / /\\  | |_/ | |_  \n"
			                                             "|_| \\| _)_) |_| \\| /_/--\\ |_| \\ |_|__", '\n'),
			                        0, 0, Colors::pair(COLOR_BLUE, COLOR_DEFAULT, true));

			this->windows[1]->print(" v" VERSION "               (built " DATE ")",
			                        0, 3,
			                        Colors::pair(COLOR_GREEN, COLOR_DEFAULT, true));

			this->windows[1]->print(Utils::String::split("Try `nsnake --help` and `man nsnake`\n"
			                                             "\n"
			                                             "Game made by Alexandre Dantas,\n"
			                                             "contact him at <eu@alexdantas.net>\n"
			                                             "Thanks for playing this game :)\n"
			                                             "\n"
			                                             "Homepage:\n"
			                                             " http://nsnake.alexdantas.net/\n"
			                                             "Source Code:\n"
			                                             " https://github.com/alexdantas/nsnake/", '\n'),
			                        0, 5, Globals::Theme::text);
		}

		this->windows[activatedIndex]->refresh();
		this->main->refresh();
		refresh();

		// Getting Input
		InputManager::update();

		if (InputManager::isPressed("left") || // user-defined
		    InputManager::isPressed(KEY_LEFT))
		{
			activatedIndex--;
			if (activatedIndex < 0)
				activatedIndex = 0;
		}
		else if (InputManager::isPressed("right") || // user-defined
		         InputManager::isPressed(KEY_RIGHT))
		{
			activatedIndex++;
			if (activatedIndex > 1)
				activatedIndex = 1;
		}
		else if (InputManager::isPressed("quit") ||
		         InputManager::isPressed(KEY_ENTER) ||
		         InputManager::isPressed('\n'))
			return;
	}
}

