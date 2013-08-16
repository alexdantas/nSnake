#ifndef NCURSES_H_DEFINED
#define NCURSES_H_DEFINED

#include <string>
#include <iostream>
#include <stdint.h>
#include <ncurses.h>
#include <unistd.h> // usleep(), suseconds_t
#include "Timer.hpp"
#include "Color.hpp"
#include "Window.hpp"

/// Shuts up the compiler about unused parameters.
#define UNUSED(x) ((void)(x))

/// Big wrapper class that handles all direct Ncurses
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
    ///
    /// @note If *emergency* is true, will leak memory and
    ///       not clean up right, but will certainly be faster
    ///       and less bug-prone.
    ///       The thing is when something on Ncurses bugs and we
    ///       try to exit properly, it generates a segmentation
    ///       fault.
    ///       Setting *emergency* to true will prevent this.
    static void exit(bool emergency=false);

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

    /// Returns a random integer between *min* and *max*
    /// (inclusive).
    static int randomNumberBetween(int min, int max);

    /// Makes the blinking cursor invisible or visible.
    static void hideCursor(bool option);

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

    // what about this?

    template<typename T>
    static T fromString(std::string s);

    template<typename T>
    static std::string toString(T s);


    // /// Rotates and/or zooms surface *src*, with *angle*, *scaleX* and
    // /// *scaleY*.
    // static Ncurses_Surface* rotoZoom(Ncurses_Surface* surface, float angle, float scaleX=1.0, float scaleY=1.0);

    // /// Returns a new copy of the *original* surface, clipped with
    // /// sizes on *clip*.
    // static Ncurses_Surface* clip(Ncurses_Surface* original, Ncurses_Rect* clip);

    /// Shows *what* on *x* *y*.
    static void print(std::string what, int x, int y);

    static void setStyle(unsigned long pair);

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

    /// Main window of the program.
    ///
    /// This is the user-definable program screen.
    ///
    /// HACK I made this public so the Input could grab it.
    /// TODO Figure out how to make things more modular.
    static Window* window;

private:
    /// The framerate. By default, 60 frames per second.
    static int framerate;

    /// How much time each frame must have according to the framerate.
    static int framerate_delay;

    /// The timer that keeps calculating the delta between frames.
    static Timer framerate_timer;

    /// How much time has passed between this and last frame (milisseconds).
    static int frame_delta;

    /// Main screen of the program.
    ///
    /// When you start ncurses, this represents the whole terminal screen.
    /// Whenever you change the terminal's size, this will be changed too.
    static WINDOW* screen;
};

#endif /* NCURSES_H_DEFINED */

