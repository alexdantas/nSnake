#ifndef MENU_H_DEFINED
#define MENU_H_DEFINED

#include <Engine/Graphics/Window.hpp>
#include <Engine/Graphics/Widgets/Menu/MenuItem.hpp>
#include <Engine/Graphics/Widgets/Menu/MenuItem/MenuItemLabel.hpp>
#include <Engine/Graphics/Widgets/Menu/MenuItem/MenuItemCheckbox.hpp>
#include <Engine/Graphics/Widgets/Menu/MenuItem/MenuItemNumberbox.hpp>
#include <Engine/Graphics/Widgets/Menu/MenuItem/MenuItemTextbox.hpp>
#include <Engine/Graphics/Widgets/Menu/MenuItem/MenuItemTextlist.hpp>

#include <vector>
#include <string>

/// List of selectable items.
///
/// @warning When adding items, make sure they're allocated
///          dynamically! I mean POINTERS, damn it.
///          Otherwise SEGFAULTS when trying to delete
///          static stuff.
///
/// @warning If you create a menu full of null elements
///          (with #addBlank()), it'll probably get into
///          an infinite loop when you call #goNext()
///          or #goPrevious.
///          I still have to find a way to fix this.
///
class Menu
{
public:
	/// Creates a menu at #x and #y with #width and #height.
	///
	/// It isn't attached to any specific Window,
	/// except when you call Menu::draw().
	Menu(int x, int y, int width, int height);

	virtual ~Menu();

	void add(MenuItem* item);
	void addBlank();

	/// Removes the menu item with #id.
	void removeByID(int id);

	/// Removes the menu item with #label.
	///
	/// @note It'll delete the first entry found with #label.
	///
	/// @warning This is far more unreliable than
	///          #removeById(), provided you have unique IDs.
	///
	void removeByLabel(std::string label);

	/// Draws the whole Menu on #window.
	void draw(Window* window);

	/// Makes the menu react to input,
	/// as seen on the global InputManager.
	void handleInput();

	/// Makes the menu select the next item.
	/// @note This is a recursive function to guarantee we'll
	///       always point to a non-nil element.
	void goNext();

	/// Makes the menu select the previous item.
	/// @note This is a recursive function to guarantee we'll
	///       always point to a non-nil element.
	void goPrevious();

	void goFirst();
	void goLast();
	void goRandom();

	/// Tells if the user selected an item that quits the menu.
	bool willQuit();

	/// Returns the label of the currently selected item.
	std::string currentLabel();

	/// Returns the user-specified id of the selected item.
	int currentID();

	/// Returns the bool internal value of item that has #id.
	/// @note Meaning the item must be a MenuItemCheckbox.
	bool getBool(int id);

	/// Returns the integer value of the item that has #id.
	/// @note Meaning the item must be a MenuItemNumberbox.
	int getInt(int id);

	/// Returns the string value of the item that has #id.
	/// @note Meaning the item must be a MenuItemTextbox or
	///       MenuItemTextlist.
	std::string getString(int id);

	/// Makes the menu able to be selected again.
	///
	/// When the user selects an item, #willQuit() returns
	/// true.
	/// By calling this function, you make possible to select
	/// things again.
	void reset();

	// FIXME: create an iterator so I dont need to expose
	//        this vector

	/// Container of all the options inside the menu.
	std::vector<MenuItem*> item;

	/// Current item selected.
	/// @note It's public because we might want to change
	///       current item's attributes. Don't mess with
	///       the pointer itself!
	MenuItem* current;

protected:
	/// Index of the currently selected item.
	unsigned int currentIndex;

	int x;
	int y;
	int width;
	int height;

	/// Tells if the user selected an item (pressed Enter).
	bool selected;

	/// Specifies which item the user pressed Enter on.
	MenuItem* selectedItem;
};

#endif //MENU_H_DEFINED

