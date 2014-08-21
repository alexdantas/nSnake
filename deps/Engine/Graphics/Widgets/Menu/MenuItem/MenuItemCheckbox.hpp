#ifndef MENUITEMCHECKBOX_H_DEFINED
#define MENUITEMCHECKBOX_H_DEFINED

#include <Engine/Graphics/Widgets/Menu/MenuItem.hpp>

/// A little box that can be checked or not.
/// Used to get a boolean from the user.
///
struct MenuItemCheckbox: public MenuItem
{
	MenuItemCheckbox(std::string label, int id, bool initial=false);

	virtual ~MenuItemCheckbox() {};

	void draw(Window* window, int x, int y, int width, bool hilite=false);

	void handleInput();

	void check(bool option);
	void toggle();
	bool isChecked();

	bool checked;
};

#endif //MENUITEMCHECKBOX_H_DEFINED

