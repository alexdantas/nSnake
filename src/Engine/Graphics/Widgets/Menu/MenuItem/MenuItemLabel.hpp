#ifndef MENUITEMLABEL_H_DEFINED
#define MENUITEMLABEL_H_DEFINED

#include <Engine/Graphics/Widgets/Menu/MenuItem.hpp>

/// Shows a Menu Item with left and right labels.
struct MenuItemLabel: public MenuItem
{
	MenuItemLabel(std::string label, int id, std::string rightLabel);
	virtual ~MenuItemLabel() {};

	void draw(Window* window, int x, int y, int width, bool hilite=false);

	void handleInput();

	void set(std::string str);

	std::string rightLabel;
};

#endif //MENUITEMLABEL_H_DEFINED

