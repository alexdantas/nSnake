#ifndef MENU_H_DEFINED
#define MENU_H_DEFINED

#include <string>
#include <vector>
#include "Color.hpp"

/// Container for individual menu item's stuff.
struct MenuItem
{
    std::string text; ///< Text that will be printed onscreen.
    int value;        ///< Item's custom value to identify itself.
    int x;
    int y;

    MenuItem(std::string text, int value, int x, int y):
        text(text),
        value(value),
        x(x),
        y(y)
    { }
};

/// Implements a menu - way to select options.
///
/// @bug TODO: Fix this bug.
///      If highlightFont size is greater than normalFont:
///      When we start the menu, by adding the first item,
///      we set it with the highlightFont.
///      So when we call centralizeText, it centralizes
///      all of the items, and the first, thinking that
///      the size of the first item is the size of it with
///      the highlightFont.
///      Finally, when we move the cursor, the first item
///      becomes non-centralized because the original
///      centralized position was calculated with the
///      other font.
///
/// @bug TODO: Fix this other bug.
///      isMouseInside() doesn't work when we consider the
///      cameraX and cameraY.
///      how can I do this?
class Menu
{
public:

    /// Creates a menu.
    Menu(unsigned long color, unsigned long hiliteColor, int x=0, int y=0);

    virtual ~Menu();

    /// Insers an item on the menu with *text* and optional
    /// custom *value*.
    ///
    /// The _value_ thing can be used to individually identify
    /// the current option on the menu.
    ///
    /// Also, you can work with the current option index. If you
    /// add items on an order, it's easy to guess what will be
    /// the first/second/whatever item - so no need for individual
    /// values.
    void addItem(std::string text, int value=-1);

    /// Returns the custom value of the current item.
    ///
    /// @return -1 if no items are set.
    int getCurrentItemValue();

    /// Returns the index of the currently selected item.
    ///
    /// @note Starts counting from zero!
    int getCurrentItemIndex();

    /// Returns how many items the menu has right now.
    int getAmmount();

    void next();     ///< Advances one item on the menu.
    void previous(); ///< Regresses one item on the menu.
    void first();    ///< Goes to the first item on the menu.
    void last();     ///< Gost to the last item on the menu.

    /// Shows the menu on screen.
    void render();

    /// Updates itself based on input.
    void update();

    /// Centralizes the text of all the items inside the menu.
    void centralizeText();

    /// Tells if the mouse was clicked inside one of the menu's items.
    ///
    /// @note To get what option was clicked, immediately call
    ///       getCurrentItemIndex() or getCurrentItemValue() because
    ///       the click is only visible on the exact frame it was
    ///       clicked.
    bool mouseClickedOnItem();

private:

    /// The color that will print the menu onscreen.
    unsigned long color;

    /// The color that will show the highlighted item.
    unsigned long hiliteColor;

    /// Contains all options inside the menu.
    std::vector<MenuItem*> item;

    /// The ammount of items the menu has right now.
    int items;

    /// Index of the currently selected item.
    int currentItem;

    float x;
    float y;
    int width;  ///< Menu's full width in pixels.
    int height; ///< Menu's full height in pixels.

    /// Flags a mouse click inside one of the items of the menu.
    ///
    /// TODO improve the interface between mouse clicks  on the menu
    ///      and the external state viewing them.
    bool mouseSelectedItem;
};

#endif //MENU_H_DEFINED

