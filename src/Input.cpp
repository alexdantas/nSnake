#include "Input.hpp"

Input* Input::instance = NULL;

Input::Input()
{
    this->will_quit = false;
    this->keyboard  = NULL;
}
void Input::update(float cameraX, float cameraY)
{
    (void)(cameraX); // If I'm not going to use, why do I have
    (void)(cameraY); // these things?

    int i;
    for (i = 0; i < (KEY_MAXIMUM + 1); i++)
        this->keyDown[i] = false;

    for (i = 0; i < 3; i++)
    {
        this->mouseDown[i] = false;
        this->mouseUp[i]   = false;
    }

    int c = getch();

    // I had an idea of using `while (c != ERR)` to get all input
    // from the supposely existing input queue.
    //
    // Everytime the user presses something, it goes to an input
    // queue. If the queue is empty, it returns ERR.
    // So we iterate through it, just in case the user pressed
    // a lot of keys at once.
    //
    // NOPE, it doesnt work.

    // Special treatment for mouse stuff
    if (c == KEY_MOUSE)
    {
        // This is VERY ugly
        // TODO improve this somewhat

        MEVENT event;

        if (getmouse(&event) != OK)
        {
            // what should I do in case of errors?
        }
        this->mouseX = event.x;
        this->mouseY = event.y;

        if (event.bstate & BUTTON1_PRESSED)
            this->mouseDown[0] = true;
        else if (event.bstate & BUTTON1_RELEASED)
            this->mouseUp[0] = true;

        else if (event.bstate & BUTTON2_PRESSED)
            this->mouseDown[1] = true;
        else if (event.bstate & BUTTON2_RELEASED)
            this->mouseUp[1] = true;

        else if (event.bstate & BUTTON3_PRESSED)
            this->mouseDown[2] = true;
        else if (event.bstate & BUTTON3_RELEASED)
            this->mouseUp[2] = true;

        else if (event.bstate & BUTTON4_PRESSED)
            this->mouseDown[3] = true;
        else if (event.bstate & BUTTON4_RELEASED)
            this->mouseUp[3] = true;

        else
        {
            // must deal with other events, look here:
            // http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/mouse.html
        }
    }
    // Hooray, an *actual* keypress!
    else
        this->keyDown[c] = true;
}
bool Input::isKeyDown(int key)
{
    return this->keyDown[key];
}
bool Input::isMouseDown(uint8_t button)
{
    if (button > 3)
        return false;

    return this->mouseDown[button];
}
bool Input::isMouseUp(uint8_t button)
{
    if (button > 3)
        return false;

    return this->mouseUp[button];
}
bool Input::isMousePressed(uint8_t button)
{
    if (button < 1 || button > 3)
        return false;

    // yes, it's a bitwise operation
    // if (this->mouse & SDL_BUTTON(button))
    //     return true;

    return false;
}
int Input::getMouseX()
{
    return (this->mouseX);
}
int Input::getMouseY()
{
    return (this->mouseY);
}
bool Input::quitRequested()
{
    return (this->will_quit);
}
bool Input::isMouseInside(int x, int y, int w, int h)
{
    if ((this->mouseX >= x) && (this->mouseX <= x + w) &&
        (this->mouseY >= y) && (this->mouseY <= y + h))
        return true;

    return false;
}
Input* Input::getInstance()
{
    if (!instance)
        instance = new Input();

    return instance;
}

