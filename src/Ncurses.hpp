#ifndef NCURSES_H_DEFINED
#define NCURSES_H_DEFINED

#include <string>
#include <iostream>
#include <stdint.h>
#include <ncurses.h>
#include <unistd.h> // usleep(), suseconds_t
#include "Timer.hpp"

// Shuts up the compiler about unused parameters.
#define UNUSED(x) ((void)(x))

/// Possible ncurses color pairs (FOREGROUND_BACKGROUND).
///  This defines all possible combinations of color pairs on ncurses.
///  Note that the order of definition is important, also the intial value
///  of 1.
///
///  To access a specific color pair, use this formula:
///    pair(n) = (foreground * 8) + background
///
///  Just like
///    pair    = (COLOR_RED * 8) + COLOR_BLUE
///
enum Colors
{
	BLACK_BLACK = 1, BLACK_RED,   BLACK_GREEN,   BLACK_YELLOW,   BLACK_BLUE,   BLACK_MAGENTA,   BLACK_CYAN,   BLACK_WHITE,
	RED_BLACK,       RED_RED,     RED_GREEN,     RED_YELLOW,     RED_BLUE,     RED_MAGENTA,     RED_CYAN,     RED_WHITE,
	GREEN_BLACK,     GREEN_RED,   GREEN_GREEN,   GREEN_YELLOW,   GREEN_BLUE,   GREEN_MAGENTA,   GREEN_CYAN,   GREEN_WHITE,
	YELLOW_BLACK,    YELLOW_RED,  YELLOW_GREEN,  YELLOW_YELLOW,  YELLOW_BLUE,  YELLOW_MAGENTA,  YELLOW_CYAN,  YELLOW_WHITE,
	BLUE_BLACK,      BLUE_RED,    BLUE_GREEN,    BLUE_YELLOW,    BLUE_BLUE,    BLUE_MAGENTA,    BLUE_CYAN,    BLUE_WHITE,
	MAGENTA_BLACK,   MAGENTA_RED, MAGENTA_GREEN, MAGENTA_YELLOW, MAGENTA_BLUE, MAGENTA_MAGENTA, MAGENTA_CYAN, MAGENTA_WHITE,
	CYAN_BLACK,      CYAN_RED,    CYAN_GREEN,    CYAN_YELLOW,    CYAN_BLUE,    CYAN_MAGENTA,    CYAN_CYAN,    CYAN_WHITE,
	WHITE_BLACK,     WHITE_RED,   WHITE_GREEN,   WHITE_YELLOW,   WHITE_BLUE,   WHITE_MAGENTA,   WHITE_CYAN,  WHITE_WHITE
};

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

    /// Returns the number of seconds since the last frame.
    ///
    /// @note It's _float_, mate.
    ///
    /// ## For developers
    ///
    /// The FPS on a powerful machine is fast and the FPS is slow
    /// on a poor computer.
    ///
    /// If on our code we have a movement like this:
    ///
    ///     Player.x += 1;
    ///
    /// Then it will move 30 pixels per second on a machine with
    /// 30 FPS and 60 pixels per second on a machine with 60 FPS
    /// (slow and fast machines, respectivelly).
    ///
    /// Now if we have a movement like this:
    ///
    ///     Player.x += 45 * dt;
    ///
    /// Then the player will move 45 pixels per second INDEPENDENT
    /// of the machine's FPS.
    ///
    /// That's because if *dt* is the ammount of seconds since last
    /// frame, if we keep doing this every frame:
    ///
    ///     float var += 1 * dt;
    ///
    /// Then after 1 second the variable will have 1 INDEPENDENT
    /// of the machine's FPS.
    ///
    static float getDeltaSeconds();

    /// Misc/Fun METHODS

    /// Returns a random integer between *min* and *max* (inclusive).
    static int randomNumberBetween(int min, int max);

    /// Makes the blinking cursor invisible or visible.
    static void hideCursor(bool option);

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

    /// Shows *what* on *x* *y*.
    static void print(std::string what, int x, int y);

    /// Updates the screen, commiting all changes made to it.
    ///
    /// @note Must be called to apply the changes on the screen.
    static void refresh();

    static void clearScreen();

    /// Pauses the program's execution for *delay* microseconds
    /// (1/1000000 of a second).
    ///
    /// ## For developers
    ///
    /// *useconds_t* is defined on *<unistd.h>* and it's used for
    /// *usleep()*.
    static void delay_us(useconds_t delay);

    /// Pauses the program's execution for *delay* miliseconds
    /// (1/1000 of a second).
    static void delay_ms(int delay);

    /// How much time we wait for getting an input.
    ///
    /// If *delay* is negative, will wait forever for input.
    /// If *delay* is zero, won't wait for input.
    /// If *delay* is positive, will wait *delay* miliseconds.
    static void inputDelay(int delay);

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

