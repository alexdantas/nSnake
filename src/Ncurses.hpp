#ifndef NCURSES_H_DEFINED
#define NCURSES_H_DEFINED

#include <string>
#include <iostream>
#include <stdint.h>
#include <ncurses.h>
#include "Timer.hpp"

// Shuts up the compiler about unused parameters.
#define UNUSED(x) ((void)(x))

/// Big wrapper class thath handles all direct Ncurses
/// calls.
///
class Ncurses
{
public:
    /// Starts all things related to ncurses.
    ///
    /// The constructor only allocates stuff, here's where the
    /// action begins.
    ///
    /// @note If this fails, will throw an exception.
    static bool init(int width=80, int height=24, int frameRate=30);

    /// Destroys anything related to ncurses.
    static void exit();

    // FRAMERATE CONTROL

    /// Returns the ammount of time each frame should have according
    /// to the framerate.
    static int getFramerateDelay();

    /// Waits as long as needed to keep the framerate, if necessary.
    /// @note Sometimes it will not sleep at all.
    static void framerateWait();

    /// Returns the ammount of time the last frame had.
    static int getDelta();

    /// Misc/Fun METHODS

    /// Returns a random integer between *min* and *max* (inclusive).
    static int randomNumberBetween(int min, int max);

    /// Makes the blinking cursor invisible.
    static void hideCursor();

    // /// Hides the mouse cursor.
    // static void hideCursor();

    // /// Shows the mouse cursor if hidden.
    // static void showCursor();

    // /// Toggles The cursor's visible state (shown/hidden).
    // static void toggleCursor();

    // /// Ejects the CD tray.
    // /// This function fails if there's not a default CD-ROM drive.
    // /// @return Bool representing success/fail.
    // static bool ejectCDTray();

    // /// Grabs (almost) all input from keyboard and mouse to the Window.
    // static void grabInput();

    // /// Releases the exclusive input from keyboard and mouse to the Window.
    // static void releaseInput();

    // /// Toggles the input-grabbed state.
    // ///
    // /// @see grabInput()
    // /// @see releaseInput()
    // static void toggleGrabInput();

    // /// Warps the mouse to the new position *x*/*y*.
    // ///
    // /// @note It generates a mouse motion event - just like if the
    // ///       user had done it.
    // static void setMousePosition(uint16_t x, uint16_t y);

    // // Weird methods that shouldn't be here

    // /// Tells if the *key* can be printed onscreen (not a control key).
    // /// If a key is visible according to the ASCII table.
    // static bool isPrintable(NcursesKey key);

    /// Converts an integer to string and return it.
    /// This is harder than it seems. In C all you had to do was call
    /// atoi(), but now it's not part of the C++ standard.
    /// So we need to create a string stream and do some mumbo-jumbo
    /// stuff to it.
    static std::string intToString(int num);

    // /// Rotates and/or zooms surface *src*, with *angle*, *scaleX* and
    // /// *scaleY*.
    // static Ncurses_Surface* rotoZoom(Ncurses_Surface* surface, float angle, float scaleX=1.0, float scaleY=1.0);

    // /// Returns a new copy of the *original* surface, clipped with
    // /// sizes on *clip*.
    // static Ncurses_Surface* clip(Ncurses_Surface* original, Ncurses_Rect* clip);

    static void print(std::string what, int x, int y);
    static void refresh();

    // Public variables
    // (please don't change them during the game)
    static int width;
    static int height;
    static int currentWidth;
    static int currentHeight;
    static bool hasColors;

private:
    /// The framerate. By default, 60 frames per second.
    static int framerate;

    /// How much time each frame must have according to the framerate.
    static int framerate_delay;

    /// The timer that keeps calculating the delta between frames.
    static Timer framerate_timer;

    /// How much time has passed between this and last frame (milisseconds).
    static int frame_delta;

    static WINDOW* screen;
};

#endif /* NCURSES_H_DEFINED */

