#include <Engine/Graphics/Widgets/Menu/MenuItem.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/InputManager.hpp>

MenuItem::MenuItem(std::string label, int id):
	type(MenuItem::ITEM),
	label(label),
	id(id)
{ }

void MenuItem::draw(Window* window, int x, int y, int width, bool hilite)
{
	window->print(this->label.substr(0, width), x, y,
	              ((hilite) ?
	               EngineGlobals::Theme::hilite_text:
	               EngineGlobals::Theme::text));
}
void MenuItem::handleInput()
{ }

