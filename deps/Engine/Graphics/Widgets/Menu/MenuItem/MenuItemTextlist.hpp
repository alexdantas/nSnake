#ifndef MENUITEMTEXTLIST_H_DEFINED
#define MENUITEMTEXTLIST_H_DEFINED

#include <Engine/Graphics/Widgets/Menu/MenuItem.hpp>

#include <vector>
#include <string>

/// A list of selectable text
class MenuItemTextlist: public MenuItem
{
public:
	MenuItemTextlist(std::string label, int id, std::vector<std::string> options, std::string initial);

	virtual ~MenuItemTextlist() { };

	void draw(Window* window, int x, int y, int width, bool hilite);
	void handleInput();

	void increase();
	void decrease();
	void reset();

	std::string currentText();

private:
	std::vector<std::string> options;
	int currentIndex;
};

#endif //MENUITEMTEXTLIST_H_DEFINED

