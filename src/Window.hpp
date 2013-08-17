#ifndef WINDOW_H_DEFINED
#define WINDOW_H_DEFINED

#include <ncurses.h>
#include <string>
#include "Shapes.hpp"

/// The borders around the window.
struct Border
{
    int left;
    int right;
    int top;
    int bottom;
    int topLeft;
    int topRight;
    int bottomLeft;
    int bottomRight;
};

/// A rectangular segment of the terminal screen.
///
///
class Window
{
public:
    Window(int x=0, int y=0, int w=1, int h=1);
    Window(Rectangle* rect);
    virtual ~Window();

//    virtual void move(int x, int y);
    virtual void resize(int w, int h);

    void print(std::string str, int x, int y);

    /// Sets the current style to *pair*.
    ///
    /// This means that anything that gets printed from now on
    /// will follow this style.
    void setStyle(unsigned long pair);

    /// Sets the window's background to *ch* with *pair*.
    ///
    /// @see Color
    void setBackground(chtype ch, unsigned long pair);

    /// Refreshes the window screen - shows whatever we told it
    /// to show.
    void refresh();

    /// Clears the screen to it's background.
    ///
    /// @see setBackground()
    void clear();

    // int getW() const;
    // int getH() const;
    // int getX() const;
    // int getY() const;

    /// Returns ncurses' internal data structure.
    ///
    /// @warning Watch out! This is very dangerous and could lead to
    /// disasters!
    /// Make sure you don't change the return pointer or undefined
    /// behaviour may happen.
    WINDOW* getWin();

    /// Sets the border of this window to the default values.
    void setBorder();

    /// Sets the border of this window do specified characters.
    void setBorder(int horizontal, int vertical, int edges);

    /// Sets the border of this window to specified characters.
    void setBorder(int left, int right, int top, int bottom,
                   int topLeft, int topRight, int bottomLeft, int bottomRight);

protected:
    /// Ncurses' internal data structure that represents a window.
    WINDOW* win;

    /// The logical position and size of the window.
    Rectangle* box;

    /// Tells if we've got some initialization error.
    bool error;

    /// Tells if we're using an external rectangle to delimit
    /// our window.
    bool usingExternalBox;

    bool hasBorder;

    Border border;
};

#endif /* WINDOW_H_DEFINED */

