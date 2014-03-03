#ifndef MENUITEMNUMBERBOX_H_DEFINED
#define MENUITEMNUMBERBOX_H_DEFINED

#include <Interface/Menu/MenuItem.hpp>

///
struct MenuItemNumberbox: public MenuItem
{
	MenuItemNumberbox(std::string label, int id, int min, int max, int initial);
	virtual ~MenuItemNumberbox() { };

	void draw(Window* window, int x, int y, int width, bool hilite=false);

	void handleInput();

	void increase();
	void decrease();
	void set(int value);
	void reset();

	int min;
	int max;
	int initial;
	int current;

private:
	/// Assures the #current value will remain within boundaries.
	void cap();
};

#endif //MENUITEMNUMBERBOX_H_DEFINED

