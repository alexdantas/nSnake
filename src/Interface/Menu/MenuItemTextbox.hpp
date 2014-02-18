#ifndef MENUITEMTEXTBOX_H_DEFINED
#define MENUITEMTEXTBOX_H_DEFINED

#include <Interface/Menu/MenuItem.hpp>

///
struct MenuItemTextbox: public MenuItem
{
	/// Creates the Widet with #initial initial id.
	MenuItemTextbox(std::string label, int id, int width, std::string initial);
	virtual ~MenuItemTextbox();

	void draw(Window* window, int x, int y, int width, bool hilite=false);

	void handleInput(int input);

	std::string currentText;
	int width;
};

#endif //MENUITEMTEXTBOX_H_DEFINED

