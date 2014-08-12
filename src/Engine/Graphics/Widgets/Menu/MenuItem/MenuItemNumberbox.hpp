#ifndef MENUITEMNUMBERBOX_H_DEFINED
#define MENUITEMNUMBERBOX_H_DEFINED

#include <Engine/Graphics/Widgets/Menu/MenuItem.hpp>

/// Allows to select a number, kinda like a slider.
///
/// There's a number and by pressing left and right you
/// add or remove units of it.
///
/// @note When you press a number, it will allow you to
///       directly set it for a while.
///       For example, pressing '1', '2' and '3' would
///       set the number first to '1', then to '12' and
///       then to '123'.
///
struct MenuItemNumberbox: public MenuItem
{
	/// Create a new number box.
	///
	/// @param label   Textual label on the left of the item
	/// @param id      Unique identifier so the menu can know which item this is
	/// @param min     Minimal allowed value for the number
	/// @param max     Maximum allowed value for the number
	/// @param initial Initial value for the number
	/// @param jump    How many units will the number jump when user press left or right
	///
	MenuItemNumberbox(std::string label, int id, int min, int max, int initial, int jump=1);
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
	int jump;

private:
	/// Assures the #current value will remain within boundaries.
	void cap();
};

#endif //MENUITEMNUMBERBOX_H_DEFINED

