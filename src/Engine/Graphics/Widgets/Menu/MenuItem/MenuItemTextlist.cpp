#include <Engine/Graphics/Widgets/Menu/MenuItem/MenuItemTextlist.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/InputManager.hpp>

#include <stdexcept>

MenuItemTextlist::MenuItemTextlist(std::string label, int id, std::vector<std::string> options, std::string initial):
	MenuItem(label, id),
	options(options),
	currentIndex(-1)
{
	this->type = MenuItem::TEXTLIST;

	for (unsigned int i = 0; i < options.size(); ++i)
	{
		if (options[i] == initial)
		{
			this->currentIndex = i;
			break;
		}
	}
	if ((this->currentIndex == -1) || (options.empty()))
		throw std::runtime_error("Invalid initial value for MenuItemTextlist");
}
void MenuItemTextlist::draw(Window* window, int x, int y, int width, bool hilite)
{
	std::string text = this->options[this->currentIndex];

	// Will draw
	//      label     text
	// If not hilite.
	// If hilite:
	//      label   < text >
	MenuItem::draw(window,
	               x,
	               y,
	               (width - text.size() - 5),
	               hilite);

	int rightmost = x + width;

	window->print(((hilite)?
	               "<":
	               "["),
	              rightmost - text.size() - 2,
	              y,
	              ((hilite)?
	               EngineGlobals::Theme::hilite_text:
	               EngineGlobals::Theme::text));

	window->print(((hilite)?
	               ">":
	               "]"),
	              rightmost - 1,
	              y,
	              ((hilite)?
	               EngineGlobals::Theme::hilite_text:
	               EngineGlobals::Theme::text));

	window->print(text,
	              rightmost - text.size() - 1,
	              y,
	              EngineGlobals::Theme::hilite_text);
}
void MenuItemTextlist::handleInput()
{
	if (InputManager::noKeyPressed())
		return;

	if (InputManager::isPressed("left") || // user-defined
		InputManager::isPressed(KEY_LEFT))
		this->decrease();

	else if (InputManager::isPressed("right") ||
	         InputManager::isPressed(KEY_RIGHT))
		this->increase();

	else if (InputManager::isPressed('r') ||
	         InputManager::isPressed('R') ||
	         InputManager::isPressed(' '))
		this->reset();
}
void MenuItemTextlist::increase()
{
	this->currentIndex++;

	if (this->currentIndex >= (int)this->options.size())
		this->currentIndex = 0;
}

void MenuItemTextlist::decrease()
{
	this->currentIndex--;

	if (this->currentIndex < 0)
		this->currentIndex = (this->options.size() - 1);
}
void MenuItemTextlist::reset()
{
	this->currentIndex = 0;
}
std::string MenuItemTextlist::currentText()
{
	return this->options[this->currentIndex];
}

