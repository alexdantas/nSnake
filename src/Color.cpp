#include <map>
#include <utility>   // pair
#include <algorithm> // transform()
#include <ncurses.h>
#include "Color.hpp"

// Static initialization of variables
bool Color::hasColors            = false;
bool Color::canChangeColors      = false;
bool Color::hasDefaultBackground = false;

// Local function.
// See below for more details.
short colorByName(const std::string& f, const std::string& b);

bool Color::init()
{
    if (has_colors() != TRUE)
        return false;

    hasColors = true;
    start_color();

    if (can_change_color() == TRUE)
        canChangeColors = true;

    // This is a big hack to initialize all possible colors
    // in ncurses. The thing is, all colors are between
    // COLOR_BLACK and COLOR_WHITE.
    // Since I've set a large number of enums covering
    // all possibilities, I can do it all in a for loop.
    // Check 'man init_pair' for more details.
    //
    // This was taken straight from <curses.h>:
    //
    // #define COLOR_BLACK   0
    // #define COLOR_RED     1
    // #define COLOR_GREEN   2
    // #define COLOR_YELLOW  3
    // #define COLOR_BLUE    4
    // #define COLOR_MAGENTA 5
    // #define COLOR_CYAN    6
    // #define COLOR_WHITE   7
    int k = 1;
    for (int i = (COLOR_BLACK); i <= (COLOR_WHITE); i++)
    {
        for (int j = (COLOR_BLACK); j <= (COLOR_WHITE); j++)
        {
            init_pair(k, i, j);
            k++;
        }
    }

    // If this terminal supports default background,
    // we're able to get the user's current background and
    // use it on the game. That's what the `-1` means over
    // there.
    //
    // Better than forcing black, right?
    if (use_default_colors() != ERR)
    {
        Color::hasDefaultBackground = true;

        init_pair(Color::BLACK_DEFAULT,   COLOR_BLACK,   -1);
        init_pair(Color::RED_DEFAULT,     COLOR_RED,     -1);
        init_pair(Color::GREEN_DEFAULT,   COLOR_GREEN,   -1);
        init_pair(Color::YELLOW_DEFAULT,  COLOR_YELLOW,  -1);
        init_pair(Color::BLUE_DEFAULT,    COLOR_BLUE,    -1);
        init_pair(Color::MAGENTA_DEFAULT, COLOR_MAGENTA, -1);
        init_pair(Color::CYAN_DEFAULT,    COLOR_CYAN,    -1);
        init_pair(Color::WHITE_DEFAULT,   COLOR_WHITE,   -1);
    }

    return true;
}
unsigned long Color::pair(const std::string& fore,
                          const std::string& back,
                          const std::string& attr)
{
    if (!(hasColors))
        return 0;

    if (back == "default")
    {
        // Falling back to black
        if (!(Color::hasDefaultBackground))
            return colorByName(fore, "black");

        if (fore == "black")   return COLOR_PAIR(Color::BLACK_DEFAULT);
        if (fore == "red")     return COLOR_PAIR(Color::RED_DEFAULT);
        if (fore == "green")   return COLOR_PAIR(Color::GREEN_DEFAULT);
        if (fore == "yellow")  return COLOR_PAIR(Color::YELLOW_DEFAULT);
        if (fore == "blue")    return COLOR_PAIR(Color::BLUE_DEFAULT);
        if (fore == "magenta") return COLOR_PAIR(Color::MAGENTA_DEFAULT);
        if (fore == "cyan")    return COLOR_PAIR(Color::CYAN_DEFAULT);
        if (fore == "white")   return COLOR_PAIR(Color::WHITE_DEFAULT);

        return 0; // invalid color
    }

    // Calling the workhorse of colors
    short num = colorByName(fore, back);

    if (num == 0) // invalid color
        return 0;

    // Converting from my enum to ncurses' color pairs
    unsigned long pair = COLOR_PAIR(num);

    // Applying attributes
    if (attr == "")
        pair |= A_NORMAL;

    else if (attr == "bold")
        pair |= A_BOLD;

    else if (attr == "underline")
        pair |= A_UNDERLINE;

    else if (attr == "reverse")
        pair |= A_REVERSE;

    else if (attr == "blink")
        pair |= A_BLINK;

    return pair;
}

// Local function that returns a color value from a color
// string.
//
// The color value is defined by the *enum Colors* on
// Color.hpp.
//
// This function was taken directly from 'ncutils'.
// It's a clever way of mapping color strings to color
// values.
//
// It has a large overhead at the first use but tries to
// keep it simple on the next accesses.
//
// Here's how it works:
// (You shouldn't worry about the color values.
//  Method Color::pair() has it all covered for you)
//
// For each BG/FG combination of the following colors:
//
// COLOR_BLACK
// COLOR_RED
// COLOR_GREEN
// COLOR_YELLOW
// COLOR_BLUE
// COLOR_MAGENTA
// COLOR_CYAN
// COLOR_WHITE
//
// in that order, A palette entry is made. The entries
// are grouped by background, not foreground. That is,
// colors 1..7 habe a bg of COLOR_BLACK, 8..15 have
// a bg of COLOR_RED, etc...
//
// The full list of colors is here:
//
// Background BLACK:
// 1 = BLACK on BLACK
// 2 = RED on BLACK
// 3 = GREEN on BLACK
// 4 = YELLOW on BLACK
// 5 = BLUE on BLACK
// 6 = MAGENTA on BLACK
// 7 = CYAN on BLACK
// 8 = WHITE on BLACK
//
// Background RED:
// 9 = BLACK on RED
// 10 = RED on RED
// 11 = GREEN on RED
// 12 = YELLOW on RED
// 13 = BLUE on RED
// 14 = MAGENTA on RED
// 15 = CYAN on RED
// 16 = WHITE on RED
//
// Background GREEN:
// 17 = BLACK on GREEN
// 18 = RED on GREEN
// 19 = GREEN on GREEN
// 20 = YELLOW on GREEN
// 21 = BLUE on GREEN
// 22 = MAGENTA on GREEN
// 23 = CYAN on GREEN
// 24 = WHITE on GREEN
//
// Background YELLOW:
// 25 = BLACK on YELLOW
// 26 = RED on YELLOW
// 27 = GREEN on YELLOW
// 28 = YELLOW on YELLOW
// 29 = BLUE on YELLOW
// 30 = MAGENTA on YELLOW
// 31 = CYAN on YELLOW
// 32 = WHITE on YELLOW
//
// Background BLUE:
// 33 = BLACK on BLUE
// 34 = RED on BLUE
// 35 = GREEN on BLUE
// 36 = YELLOW on BLUE
// 37 = BLUE on BLUE
// 38 = MAGENTA on BLUE
// 39 = CYAN on BLUE
// 40 = WHITE on BLUE
//
// Background MAGENTA:
// 41 = BLACK on MAGENTA
// 42 = RED on MAGENTA
// 43 = GREEN on MAGENTA
// 44 = YELLOW on MAGENTA
// 45 = BLUE on MAGENTA
// 46 = MAGENTA on MAGENTA
// 47 = CYAN on MAGENTA
// 48 = WHITE on MAGENTA
//
// Background CYAN:
// 49 = BLACK on CYAN
// 50 = RED on CYAN
// 51 = GREEN on CYAN
// 52 = YELLOW on CYAN
// 53 = BLUE on CYAN
// 54 = MAGENTA on CYAN
// 55 = CYAN on CYAN
// 56 = WHITE on CYAN
//
// Background WHITE:
// 57 = BLACK on WHITE
// 58 = RED on WHITE
// 59 = GREEN on WHITE
// 60 = YELLOW on WHITE
// 61 = BLUE on WHITE
// 62 = MAGENTA on WHITE
// 63 = CYAN on WHITE
// 64 = WHITE on WHITE
//
short colorByName(const std::string& f,
                  const std::string& b)
{
    /// TODO BUG HACK WHATEVER
    /// I have to figure out why do I have to switch
    /// background/foreground for them to work...
    /// DAMN
    std::string fore = b;
    std::string back = f;

    std::transform(fore.begin(), fore.end(),
                   fore.begin(), (int(*)(int)) std::toupper);

    std::transform(back.begin(), back.end(),
                   back.begin(), (int(*)(int)) std::toupper);

    typedef std::pair<std::string,std::string> SP;
    typedef std::map< SP, short > MT;

    static MT bob;
    if (bob.empty())
    {
        // Building the map for the first time.
        bob[SP("BLACK",   "BLACK")]   = 1;
        bob[SP("RED",     "BLACK")]   = 2;
        bob[SP("GREEN",   "BLACK")]   = 3;
        bob[SP("YELLOW",  "BLACK")]   = 4;
        bob[SP("BLUE",    "BLACK")]   = 5;
        bob[SP("MAGENTA", "BLACK")]   = 6;
        bob[SP("CYAN",    "BLACK")]   = 7;
        bob[SP("WHITE",   "BLACK")]   = 8;
        bob[SP("BLACK",   "RED")]     = 9;
        bob[SP("RED",     "RED")]     = 10;
        bob[SP("GREEN",   "RED")]     = 11;
        bob[SP("YELLOW",  "RED")]     = 12;
        bob[SP("BLUE",    "RED")]     = 13;
        bob[SP("MAGENTA", "RED")]     = 14;
        bob[SP("CYAN",    "RED")]     = 15;
        bob[SP("WHITE",   "RED")]     = 16;
        bob[SP("BLACK",   "GREEN")]   = 17;
        bob[SP("RED",     "GREEN")]   = 18;
        bob[SP("GREEN",   "GREEN")]   = 19;
        bob[SP("YELLOW",  "GREEN")]   = 20;
        bob[SP("BLUE",    "GREEN")]   = 21;
        bob[SP("MAGENTA", "GREEN")]   = 22;
        bob[SP("CYAN",    "GREEN")]   = 23;
        bob[SP("WHITE",   "GREEN")]   = 24;
        bob[SP("BLACK",   "YELLOW")]  = 25;
        bob[SP("RED",     "YELLOW")]  = 26;
        bob[SP("GREEN",   "YELLOW")]  = 27;
        bob[SP("YELLOW",  "YELLOW")]  = 28;
        bob[SP("BLUE",    "YELLOW")]  = 29;
        bob[SP("MAGENTA", "YELLOW")]  = 30;
        bob[SP("CYAN",    "YELLOW")]  = 31;
        bob[SP("WHITE",   "YELLOW")]  = 32;
        bob[SP("BLACK",   "BLUE")]    = 33;
        bob[SP("RED",     "BLUE")]    = 34;
        bob[SP("GREEN",   "BLUE")]    = 35;
        bob[SP("YELLOW",  "BLUE")]    = 36;
        bob[SP("BLUE",    "BLUE")]    = 37;
        bob[SP("MAGENTA", "BLUE")]    = 38;
        bob[SP("CYAN",    "BLUE")]    = 39;
        bob[SP("WHITE",   "BLUE")]    = 40;
        bob[SP("BLACK",   "MAGENTA")] = 41;
        bob[SP("RED",     "MAGENTA")] = 42;
        bob[SP("GREEN",   "MAGENTA")] = 43;
        bob[SP("YELLOW",  "MAGENTA")] = 44;
        bob[SP("BLUE",    "MAGENTA")] = 45;
        bob[SP("MAGENTA", "MAGENTA")] = 46;
        bob[SP("CYAN",    "MAGENTA")] = 47;
        bob[SP("WHITE",   "MAGENTA")] = 48;
        bob[SP("BLACK",   "CYAN")]    = 49;
        bob[SP("RED",     "CYAN")]    = 50;
        bob[SP("GREEN",   "CYAN")]    = 51;
        bob[SP("YELLOW",  "CYAN")]    = 52;
        bob[SP("BLUE",    "CYAN")]    = 53;
        bob[SP("MAGENTA", "CYAN")]    = 54;
        bob[SP("CYAN",    "CYAN")]    = 55;
        bob[SP("WHITE",   "CYAN")]    = 56;
        bob[SP("BLACK",   "WHITE")]   = 57;
        bob[SP("RED",     "WHITE")]   = 58;
        bob[SP("GREEN",   "WHITE")]   = 59;
        bob[SP("YELLOW",  "WHITE")]   = 60;
        bob[SP("BLUE",    "WHITE")]   = 61;
        bob[SP("MAGENTA", "WHITE")]   = 62;
        bob[SP("CYAN",    "WHITE")]   = 63;
        bob[SP("WHITE",   "WHITE")]   = 64;
    }

    SP p(fore,back);

    MT::const_iterator cit = bob.find(p);

    if (bob.end() == cit)
        return 0;

    return (*cit).second;
}

