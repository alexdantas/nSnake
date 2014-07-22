#include <Interface/Dialog.hpp>
#include <Interface/Layout.hpp>
#include <Interface/Menu/Menu.hpp>
#include <Interface/Ncurses.hpp>
#include <Config/Globals.hpp>
#include <Flow/InputManager.hpp>

#include <vector>

void Dialog::show(std::string message, bool pressAnyKey)
{
	int windowx = Layout::screenWidth/2 - (message.size() + 2)/2;
	int windowy = Layout::screenHeight/2 - 3/2;

	Window dialog(windowx,
	              windowy,
	              message.size() + 2, // borders + empty space
	              3);

	if (Globals::Screen::show_borders)
	{
		dialog.borders(Globals::Screen::fancy_borders ?
		               Window::BORDER_FANCY :
		               Window::BORDER_REGULAR);
	}

	// Before showing anything on the screen we must
	// call `refresh()`, to... well, refresh the
	// main screen buffer
	refresh();

	dialog.print(message, 1, 1);
	dialog.refresh();
	refresh();

	// Wait forever to get any key...
	if (pressAnyKey)
		Ncurses::getInput(-1);
}

bool Dialog::askBool(std::string question, std::string title, bool default_value)
{
	int windowx = Layout::screenWidth/2 - (question.size() + 12)/2;
	int windowy = Layout::screenHeight/2 - 5/2;

	Window dialog(windowx,
	              windowy,
	              question.size() + 2 + 10, // borders + empty space
	              5);

	if (Globals::Screen::show_borders)
	{
		dialog.borders(Globals::Screen::fancy_borders ?
		               Window::BORDER_FANCY :
		               Window::BORDER_REGULAR);
	}
	if (! title.empty())
		dialog.setTitle(title);

	dialog.refresh();

	Menu menu(1, 2, question.size() + 10, 2);

	std::vector<std::string> options;
	options.push_back("Yes");
	options.push_back("No");

	MenuItemTextlist* list = new MenuItemTextlist(question,
	                                              0,
	                                              options,
	                                              (default_value ?
	                                               "Yes" :
	                                               "No"));
	menu.add(list);

	while (true)
	{
		// Drawing things
		dialog.clear();

		menu.draw(&dialog);

		dialog.refresh();
		refresh();

		// Getting input (waiting infinitely for it)
		InputManager::update(-1);

		if (InputManager::isPressed("quit")) // user-defined
			return false;

		menu.handleInput();

		if (InputManager::isPressed('\n') ||
		    InputManager::isPressed(KEY_ENTER))
		{
			std::string str(menu.getString(0));
			return (str == "Yes");
		}
	}

	// Will never get here
	return false;
}

