#ifndef MENUITEMTEXTBOX_H_DEFINED
#define MENUITEMTEXTBOX_H_DEFINED

#include <Engine/Graphics/Widgets/Menu/MenuItem.hpp>

/// Place where you can input characters.
///
/// For more info, check out MenuItem.
struct MenuItemTextbox: public MenuItem
{
	MenuItemTextbox(std::string label, int id, int width, std::string initial);

	virtual ~MenuItemTextbox() {};

	void draw(Window* window, int x, int y, int width, bool hilite=false);

	void handleInput();

	std::string currentText;
	int width;
};

#endif //MENUITEMTEXTBOX_H_DEFINED

