#ifndef MENUITEMCHECKBOX_H_DEFINED
#define MENUITEMCHECKBOX_H_DEFINED

#include <Interface/Menu/MenuItem.hpp>

///
struct MenuItemCheckbox: public MenuItem
{
	MenuItemCheckbox(std::string label, int id, bool initial=false);
	virtual ~MenuItemCheckbox() { };


	void draw(Window* window, int x, int y, int width, bool hilite=false);

	void handleInput(int input);

	void check(bool option);
	void toggle();
	bool isChecked();

	bool checked;
};

#endif //MENUITEMCHECKBOX_H_DEFINED

