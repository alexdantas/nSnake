#ifndef INPUT_H_DEFINED
#define INPUT_H_DEFINED

#include <ncurses.h>

#include <iostream> // for NULL
#include <stdint.h>

/// What is the smallest value that we can obtain for a key
//  KEY_MIN is a constant defined on curses.h that tells us the
#define KEY_MINIMUM KEY_MIN

/// What is the biggest value that we can obtain for a key
//  KEY_MAX is a constant defined on curses.h that tells us the
#define KEY_MAXIMUM KEY_MAX

/// Handles input through keyboard and mouse.
//  It's the interface between ncurses-direct input calls.
//
//  @note This class is a Singleton.
//  Tips on how to implement a Singleton:
//  http://stackoverflow.com/a/271104
//
//  I TRIED IMPLEMENTING A SINGLETON BUT IT JUST KEEPS GETTING
//  ERRORS ON PRIVATE CONSTRUCTORS ARRGHHH
//
//
class Input
{
public:
    /// Returns the singleton instance of this class.
    //  This assures that only a single instance of this class is
    //  created at any point on the game.
    static Input* getInstance();

    /// Updates the entire keystates.
    void update(float cameraX=0, float cameraY=0);

    /// Tells if a keyboard button was released just now.
    //  To see all possibilities, see isKeyPressed().
    bool isKeyDown(int key);

    /// Tells if a mouse button was released just now.
    //  To see all possibilities, see isMousePressed().
    bool isMouseDown(uint8_t button);

    /// Tells if a mouse button was pressed just now.
    //  To see all possibilities, see isMousePressed().
    bool isMouseUp(uint8_t button);

    /// Tells if a key is currently being pressed.
    //  To see all available keys, visit this site:
    //  http://wiki.libsdl.org/moin.fcg/SDL_Keycode
    bool isKeyPressed(int key);

    /// Tells if a mouse button is currently being pressed.
    //  Possible options are:
    //  * 1: left   mouse button
    //  * 2: middle mouse button
    //  * 3: right  mouse button
    bool isMousePressed(uint8_t button);

    /// Tells if the user asked the game to quit.
    bool quitRequested();

    /// Returns the current mouse X position.
    int getMouseX();

    /// Returns the current mouse Y position.
    int getMouseY();

    /// Tells if the mouse's currently inside the rectangle specified
    //  by #x, #y, #w and #h.
    bool isMouseInside(int x, int y, int w, int h);

    // Everything must be private and without implementation to assure
    // new instances will never be created anywhere.
    Input();
//    ~Input() {};
    Input(Input const&) {};
    void operator=(Input const&) {};

private:
    /// Current singleton instance
    static Input* instance;

    /// Saves SDL internal mouse state.
    uint8_t  mouse;

    /// Saves SDL internal keyboard state.
    uint8_t* keyboard;

    /// Current mouse X position.
    int  mouseX;

    /// Current mouse Y position.
    int  mouseY;

    /// Saves which keys are currently down.
    //  The size (KEY_MAX) is defined on curses.h
    bool keyDown[KEY_MAXIMUM + 1];

    /// Saves which keys are currently up.
    //  The size (KEY_MAX) is defined on curses.h
    bool keyUp[KEY_MAXIMUM + 1];

    /// Saves which mouse buttons are currently down.
    bool mouseDown[3];

    /// Saves which mouse buttons are currently up.
    bool mouseUp[3];

    /// Tells if we must quit the game.
    bool will_quit;
};

#endif /* INPUT_H_DEFINED */

