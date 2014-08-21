#include <Engine/Graphics/Widgets/Menu/MenuItem/MenuItemLabel.hpp>
#include <Engine/EngineGlobals.hpp>

MenuItemLabel::MenuItemLabel(std::string label, int id, std::string rightLabel):
	MenuItem(label, id),
	rightLabel(rightLabel)
{
	this->type = MenuItem::LABEL;
}

void MenuItemLabel::draw(Window* window, int x, int y, int width, bool hilite)
{
	unsigned int rightLabelSize = this->rightLabel.size();

	MenuItem::draw(window, x, y, width - rightLabelSize - 1, hilite);

	window->print(this->rightLabel,
	              x + width - rightLabelSize,
	              y,
	              ((hilite) ?
	               EngineGlobals::Theme::hilite_text:
	               EngineGlobals::Theme::text));
}

void MenuItemLabel::handleInput()
{ }

void MenuItemLabel::set(std::string str)
{
	this->rightLabel = str;
}

