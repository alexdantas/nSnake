#ifndef MENUITEM_H_DEFINED
#define MENUITEM_H_DEFINED

#include <Engine/Graphics/Window.hpp>

#include <string>

/// Simplest type of item possible, with a label
/// and user-defined id.
///
struct MenuItem
{
	/// All possible item types.
	/// When inheriting and creating your own, make sure to
	/// place it here.
	///
	enum MenuItemType
	{
		ITEM, LABEL, CHECKBOX, NUMBERBOX, TEXTBOX, TEXTLIST
	};

	/// Create a MenuItem, with user-defined #id.
	///
	/// @note MenuItems on the same menu should have
	///       different #id s! If not, results are unpredictable.
	MenuItem(std::string label, int id);

	virtual ~MenuItem() { };

	/// Shows this item at #x, #y with #width.
	///
	/// If this is the current item, send #hilite as true.
	///
	/// @note Yeah, height is always 1.
	virtual void draw(Window* window, int x, int y, int width, bool hilite=false);

	/// Makes the menu item react to input,
	/// as seen on the global InputManager.
	///
	/// Each type of MenuItem might react to input differently.
	/// A text box might want to show printable characters,
	/// a check box might want to check if space bar was pressed,
	/// whatever.
	///
	/// When inheriting this, make sure to implement it.
	virtual void handleInput();

	/// Specific type of this widget.
	/// @note It is set on the Constructor,
	///       don't change it manually!
	MenuItemType type;

	/// Text that will be shown on the screen.
	std::string label;

	/// User-defined id to identify this item.
	int id;
};

#endif //MENUITEM_H_DEFINED

