#ifndef MENUITEMNUMBERBOX_H_DEFINED
#define MENUITEMNUMBERBOX_H_DEFINED

#include <Interface/Menu/MenuItem.hpp>

///
struct MenuItemNumberbox: public MenuItem
{
	MenuItemNumberbox(std::string label, int id, int min, int max, int initial);
	virtual ~MenuItemNumberbox() { };

	void draw(Window* window, int x, int y, int width, bool hilite=false);

	void handleInput(int input);

	void increase();
	void decrease();
	void reset();

	int min;
	int max;
	int initial;
	int current;
};

#endif //MENUITEMNUMBERBOX_H_DEFINED

