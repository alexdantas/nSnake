#include <Engine/Graphics/Widgets/Menu/MenuItem/MenuItemTextbox.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/InputManager.hpp>
#include <Engine/Helpers/Utils.hpp>

MenuItemTextbox::MenuItemTextbox(std::string label, int id, int width, std::string initial):
	MenuItem(label, id),
	currentText(initial),
	width(width)
{

}
void MenuItemTextbox::draw(Window* window, int x, int y, int width, bool hilite)
{
	// Drawing label before actual textbox
	MenuItem::draw(window, x, y, width - (this->width) - 1, hilite);

	// Maximum x id
	int posx = x + width;

	// Drawing the current text
	if ((int)(this->currentText.size()) > (this->width))
		window->print(this->currentText.substr(this->width), (posx - this->width), y, EngineGlobals::Theme::textbox);
	else
		window->print(this->currentText, (posx - this->width), y, EngineGlobals::Theme::textbox);

	int spacex = (posx - this->width) - this->currentText.size();

	// Drawing the rest of the spaces
	for (unsigned int i = 0; i < (this->width - this->currentText.size()); i++)
		window->printChar(' ', spacex + i, y, EngineGlobals::Theme::textbox);
}

// Local function that tells in #input can be interpreted
// as a printable char (meaning no control character,
// as seen on the ASCII table).
bool isPrintable(int input)
{
	return ((input > 32) && (input < 127));
}

void MenuItemTextbox::handleInput()
{
	if (InputManager::noKeyPressed())
		return;

	if (isPrintable(InputManager::pressedKey))
	{
		this->currentText += (char)(InputManager::pressedKey);
		return;
	}

	if (InputManager::isPressed(KEY_BACKSPACE))
		Utils::String::pop_back(this->currentText);
}

