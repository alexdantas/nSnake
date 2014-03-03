#include <Interface/Menu/MenuItem.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>
#include <Flow/InputManager.hpp>

MenuItem::MenuItem(std::string label, int id):
	type(MenuItem::ITEM),
	label(label),
	id(id)
{ }

void MenuItem::draw(Window* window, int x, int y, int width, bool hilite)
{
	window->print(this->label.substr(0, width), x, y,
	              ((hilite) ?
	               Globals::Theme::hilite_text:
	               Globals::Theme::text));
}
void MenuItem::handleInput()
{ }

