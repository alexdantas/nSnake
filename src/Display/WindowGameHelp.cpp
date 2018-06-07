#include <Display/WindowGameHelp.hpp>
#include <Engine/Graphics/Layout.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/InputManager.hpp>
#include <Engine/Helpers/Utils.hpp>


#include <stdio.h>
#include <stdlib.h>
#include <libintl.h>
#include <locale.h>
#define _(String) gettext (String)

WindowGameHelp::WindowGameHelp()
{
	int width  = 40;
	int height = 17;

	int windowx = Layout::screenWidth/2  - width/2;
	int windowy = Layout::screenHeight/2 - height/2;

	this->main = new Window(windowx, windowy, width, height);

	Window* win;

	// Help
	win = new Window(this->main, 0, 0, WINDOW_FILL, WINDOW_FILL);
	win->borders(Window::BORDER_NONE);

	this->windows.push_back(win);

	win = new Window(this->main, 0, 0, WINDOW_FILL, WINDOW_FILL);
	win->borders(Window::BORDER_NONE);
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
		                   _(" Help ")),
		                  2,
		                  0,
		                  ((activatedIndex == 0) ?
		                   EngineGlobals::Theme::textbox :
		                   EngineGlobals::Theme::hilite_text));

		this->main->print(((activatedIndex == 1) ?
		                   "(Credits)" :
		                   _(" Credits ")),
		                  12,
		                  0,
		                  ((activatedIndex == 1) ?
		                   EngineGlobals::Theme::textbox :
		                   EngineGlobals::Theme::hilite_text));

		// This is a HACK to avoid the borders from being
		// bold. Apparently I need to activate the default
		// ColorPair before continuing.
		//
		// TODO: Find out about this mistery.
		this->main->print("", 0, 0, EngineGlobals::Theme::text);

		// Help Window
		if (activatedIndex == 0)
		{
			this->windows[0]->print(_("In-game controls:\n"),
			                        0, 0,
			                        EngineGlobals::Theme::hilite_text);

			this->windows[0]->print(Utils::String::split(_("Move up\n"
			                                             "Move down\n"
			                                             "Move left\n"
			                                             "Move right\n"
			                                             "Pause game\n"
			                                             "Quit anytime\n"
			                                             "Show help"), '\n'),
			                        1, 1,
			                        EngineGlobals::Theme::hilite_text);

			this->windows[0]->print(Utils::String::split(InputManager::keyToString(InputManager::getBind(_("up"))) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind(_("down"))) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind(_("left"))) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind(_("right"))) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind(_("pause"))) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind(_("quit"))) + "\n" +
			                                             InputManager::keyToString(InputManager::getBind(_("help"))), '\n'),
			                        14, 1,
			                        EngineGlobals::Theme::text);

			this->windows[0]->print("Menu controls:\n",
			                        0, 9,
			                        EngineGlobals::Theme::hilite_text);

			this->windows[0]->print(Utils::String::split(_("First item\n"
			                                             "Last item"), '\n'),
			                        1, 10,
			                        EngineGlobals::Theme::hilite_text);

			this->windows[0]->print(Utils::String::split(_("page up\n"
			                                             "page down"), '\n'),
			                        14, 10,
			                        EngineGlobals::Theme::text);

			this->windows[0]->print(Utils::String::split(_(" Settings and scores are stored at:\n"
			                                             " `~/.local/share/nsnake/`"), '\n'),
			                        0, 13,
			                        EngineGlobals::Theme::text);
		}
		//
		// Credits
		else if (activatedIndex == 1)
		{
			this->windows[1]->print(Utils::String::split(" _      __   _       __    _     ____ \n"
			                                             "| |\\ | ( (` | |\\ |  / /\\  | |_/ | |_  \n"
			                                             "|_| \\| _)_) |_| \\| /_/--\\ |_| \\ |_|__", '\n'),
			                        0, 0, Colors::pair("blue", "default", true));

			this->windows[1]->print(" v" VERSION "               (built " DATE ")",
			                        0, 3,
			                        Colors::pair("green", "default", true));

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
			                        0, 5, EngineGlobals::Theme::text);
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
