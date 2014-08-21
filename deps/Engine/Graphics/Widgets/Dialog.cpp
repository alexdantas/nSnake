#include <Engine/Graphics/Widgets/Dialog.hpp>
#include <Engine/Graphics/Layout.hpp>
#include <Engine/Graphics/Widgets/Menu.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/InputManager.hpp>
#include <Engine/Helpers/Utils.hpp>

#include <vector>
#include <algorithm>

void Dialog::show(std::string message, bool pressAnyKey)
{
	std::vector<std::string> message_lines = Utils::String::split(message, '\n');

	// The dialog needs to wrap around this text. So we need...
	int message_width  = 0; // ...the char count of the widest line and...
	int message_height = 0; // ...the number of lines of the whole message

	message_height = message_lines.size();

	for (size_t i = 0; i < message_lines.size(); i++)
		message_width = std::max(message_width, (int)message_lines[i].size());

	// Now, to the size and position of the actual Dialog.
	// Making it centered on the screen
	int window_x      = Layout::screenWidth /2 - (message_width + 2)/2;
	int window_y      = Layout::screenHeight/2 - (message_height)   /2;
	int window_width  = message_width  + 2; // left/right borders
	int window_height = message_height + 2; // top/bottom borders

	Window dialog(window_x, window_y, window_width, window_height);

	// Before showing anything on the screen we must
	// call `refresh()`, to... well, refresh the
	// main screen buffer
	refresh();

	// Show all lines, starting from (1, 1)
	for (size_t i = 0; i < message_lines.size(); i++)
		dialog.print(message_lines[i], 1, i + 1);

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

