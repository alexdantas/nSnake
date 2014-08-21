#include <Engine/Graphics/Widgets/Menu/MenuItem/MenuItemCheckbox.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/InputManager.hpp>

MenuItemCheckbox::MenuItemCheckbox(std::string label, int id, bool initial):
	MenuItem(label, id),
	checked(initial)
{
	this->type = MenuItem::CHECKBOX; // placing it above wont work
}

void MenuItemCheckbox::draw(Window* window, int x, int y, int width, bool hilite)
{
	// Will draw
	//      label     text
	// If not hilite.
	// If hilite:
	//      label   < text >
	MenuItem::draw(window, x, y, width - 8, hilite); // button width

	int posx = x + width;

	window->print(((hilite)?
	               "<":
	               "["),
	              posx - 8, y,
	              ((hilite)?
	               EngineGlobals::Theme::hilite_text:
	               EngineGlobals::Theme::text));

	window->print("ON", posx - 7, y, ((this->checked) ?
	                                  EngineGlobals::Theme::hilite_text:
	                                  EngineGlobals::Theme::text));

	window->print("|", posx - 5, y, EngineGlobals::Theme::text);

	window->print("OFF", posx - 4, y, ((this->checked) ?
	                                   EngineGlobals::Theme::text :
	                                   EngineGlobals::Theme::hilite_text));

	window->print(((hilite)?
	               ">":
	               "]"),
	              posx - 1, y,
	              ((hilite)?
	               EngineGlobals::Theme::hilite_text:
	               EngineGlobals::Theme::text));
}
void MenuItemCheckbox::handleInput()
{
	if (InputManager::noKeyPressed())
		return;

	if (InputManager::isPressed("left") || // user-defined
		InputManager::isPressed(KEY_LEFT))
		this->check(true);

	else if (InputManager::isPressed("right") || // user-defined
	         InputManager::isPressed(KEY_RIGHT))
		this->check(false);

	else if (InputManager::isPressed(' ')  ||
	         InputManager::isPressed('\n') ||
	         InputManager::isPressed(KEY_ENTER))
		this->toggle();
}
void MenuItemCheckbox::check(bool option)
{
	this->checked = option;
}
void MenuItemCheckbox::toggle()
{
	this->checked = !(this->checked);
}
bool MenuItemCheckbox::isChecked()
{
	return this->checked;
}
