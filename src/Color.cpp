#include <map>
#include <utility>   // pair
#include <algorithm> // transform()
#include <ncurses.h>
#include "Color.hpp"

bool Color::hasColors       = false;
bool Color::canChangeColors = false;

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

    // Now we're able to get the user's current background and
    // use it on the game. That's what the `-1` means over
    // there.
    // Better than forcing black, right?
    if (use_default_colors() != ERR)
    {
        init_pair(BLACK_BLACK,   COLOR_BLACK,   -1);
        init_pair(RED_BLACK,     COLOR_RED,     -1);
        init_pair(GREEN_BLACK,   COLOR_GREEN,   -1);
        init_pair(YELLOW_BLACK,  COLOR_YELLOW,  -1);
        init_pair(BLUE_BLACK,    COLOR_BLUE,    -1);
        init_pair(MAGENTA_BLACK, COLOR_MAGENTA, -1);
        init_pair(CYAN_BLACK,    COLOR_CYAN,    -1);
        init_pair(WHITE_BLACK,   COLOR_WHITE,   -1);
    }

    // init_pair(1, COLOR_BLACK, COLOR_BLACK);
    // init_pair(2, COLOR_RED, COLOR_BLACK);
    // init_pair(3, COLOR_GREEN, COLOR_BLACK);
    // init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    // init_pair(5, COLOR_BLUE, COLOR_BLACK);
    // init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    // init_pair(7, COLOR_CYAN, COLOR_BLACK);
    // init_pair(8, COLOR_WHITE, COLOR_BLACK);
    // init_pair(9, COLOR_BLACK, COLOR_RED);
    // init_pair(10, COLOR_RED, COLOR_RED);
    // init_pair(11, COLOR_GREEN, COLOR_RED);
    // init_pair(12, COLOR_YELLOW, COLOR_RED);
    // init_pair(13, COLOR_BLUE, COLOR_RED);
    // init_pair(14, COLOR_MAGENTA, COLOR_RED);
    // init_pair(15, COLOR_CYAN, COLOR_RED);
    // init_pair(16, COLOR_WHITE, COLOR_RED);
    // init_pair(17, COLOR_BLACK, COLOR_GREEN);
    // init_pair(18, COLOR_RED, COLOR_GREEN);
    // init_pair(19, COLOR_GREEN, COLOR_GREEN);
    // init_pair(20, COLOR_YELLOW, COLOR_GREEN);
    // init_pair(21, COLOR_BLUE, COLOR_GREEN);
    // init_pair(22, COLOR_MAGENTA, COLOR_GREEN);
    // init_pair(23, COLOR_CYAN, COLOR_GREEN);
    // init_pair(24, COLOR_WHITE, COLOR_GREEN);
    // init_pair(25, COLOR_BLACK, COLOR_YELLOW);
    // init_pair(26, COLOR_RED, COLOR_YELLOW);
    // init_pair(27, COLOR_GREEN, COLOR_YELLOW);
    // init_pair(28, COLOR_YELLOW, COLOR_YELLOW);
    // init_pair(29, COLOR_BLUE, COLOR_YELLOW);
    // init_pair(30, COLOR_MAGENTA, COLOR_YELLOW);
    // init_pair(31, COLOR_CYAN, COLOR_YELLOW);
    // init_pair(32, COLOR_WHITE, COLOR_YELLOW);
    // init_pair(33, COLOR_BLACK, COLOR_BLUE);
    // init_pair(34, COLOR_RED, COLOR_BLUE);
    // init_pair(35, COLOR_GREEN, COLOR_BLUE);
    // init_pair(36, COLOR_YELLOW, COLOR_BLUE);
    // init_pair(37, COLOR_BLUE, COLOR_BLUE);
    // init_pair(38, COLOR_MAGENTA, COLOR_BLUE);
    // init_pair(39, COLOR_CYAN, COLOR_BLUE);
    // init_pair(40, COLOR_WHITE, COLOR_BLUE);
    // init_pair(41, COLOR_BLACK, COLOR_MAGENTA);
    // init_pair(42, COLOR_RED, COLOR_MAGENTA);
    // init_pair(43, COLOR_GREEN, COLOR_MAGENTA);
    // init_pair(44, COLOR_YELLOW, COLOR_MAGENTA);
    // init_pair(45, COLOR_BLUE, COLOR_MAGENTA);
    // init_pair(46, COLOR_MAGENTA, COLOR_MAGENTA);
    // init_pair(47, COLOR_CYAN, COLOR_MAGENTA);
    // init_pair(48, COLOR_WHITE, COLOR_MAGENTA);
    // init_pair(49, COLOR_BLACK, COLOR_CYAN);
    // init_pair(50, COLOR_RED, COLOR_CYAN);
    // init_pair(51, COLOR_GREEN, COLOR_CYAN);
    // init_pair(52, COLOR_YELLOW, COLOR_CYAN);
    // init_pair(53, COLOR_BLUE, COLOR_CYAN);
    // init_pair(54, COLOR_MAGENTA, COLOR_CYAN);
    // init_pair(55, COLOR_CYAN, COLOR_CYAN);
    // init_pair(56, COLOR_WHITE, COLOR_CYAN);
    // init_pair(57, COLOR_BLACK, COLOR_WHITE);
    // init_pair(58, COLOR_RED, COLOR_WHITE);
    // init_pair(59, COLOR_GREEN, COLOR_WHITE);
    // init_pair(60, COLOR_YELLOW,  COLOR_WHITE);
    // init_pair(61, COLOR_BLUE,    COLOR_WHITE);
    // init_pair(62, COLOR_MAGENTA, COLOR_WHITE);
    // init_pair(63, COLOR_CYAN,    COLOR_WHITE);
    // init_pair(64, COLOR_WHITE,   COLOR_WHITE);

    return true;
}


/// Local function that returns an internal number of the
/// color pair based on the names.
short byName(const std::string& f,
             const std::string& b)
{
    // TODO BUG HACK WHATEVER
    // I don't know why I have to switch these, but it
    // seems that if I don't do this things will go backwards.
    // Someday I must find a fix for this.
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
        bob[SP("BLACK","BLACK")] = 1;
        bob[SP("RED","BLACK")] = 2;
        bob[SP("GREEN","BLACK")] = 3;
        bob[SP("YELLOW","BLACK")] = 4;
        bob[SP("BLUE","BLACK")] = 5;
        bob[SP("MAGENTA","BLACK")] = 6;
        bob[SP("CYAN","BLACK")] = 7;
        bob[SP("WHITE","BLACK")] = 8;
        bob[SP("BLACK","RED")] = 9;
        bob[SP("RED","RED")] = 10;
        bob[SP("GREEN","RED")] = 11;
        bob[SP("YELLOW","RED")] = 12;
        bob[SP("BLUE","RED")] = 13;
        bob[SP("MAGENTA","RED")] = 14;
        bob[SP("CYAN","RED")] = 15;
        bob[SP("WHITE","RED")] = 16;
        bob[SP("BLACK","GREEN")] = 17;
        bob[SP("RED","GREEN")] = 18;
        bob[SP("GREEN","GREEN")] = 19;
        bob[SP("YELLOW","GREEN")] = 20;
        bob[SP("BLUE","GREEN")] = 21;
        bob[SP("MAGENTA","GREEN")] = 22;
        bob[SP("CYAN","GREEN")] = 23;
        bob[SP("WHITE","GREEN")] = 24;
        bob[SP("BLACK","YELLOW")] = 25;
        bob[SP("RED","YELLOW")] = 26;
        bob[SP("GREEN","YELLOW")] = 27;
        bob[SP("YELLOW","YELLOW")] = 28;
        bob[SP("BLUE","YELLOW")] = 29;
        bob[SP("MAGENTA","YELLOW")] = 30;
        bob[SP("CYAN","YELLOW")] = 31;
        bob[SP("WHITE","YELLOW")] = 32;
        bob[SP("BLACK","BLUE")] = 33;
        bob[SP("RED","BLUE")] = 34;
        bob[SP("GREEN","BLUE")] = 35;
        bob[SP("YELLOW","BLUE")] = 36;
        bob[SP("BLUE","BLUE")] = 37;
        bob[SP("MAGENTA","BLUE")] = 38;
        bob[SP("CYAN","BLUE")] = 39;
        bob[SP("WHITE","BLUE")] = 40;
        bob[SP("BLACK","MAGENTA")] = 41;
        bob[SP("RED","MAGENTA")] = 42;
        bob[SP("GREEN","MAGENTA")] = 43;
        bob[SP("YELLOW","MAGENTA")] = 44;
        bob[SP("BLUE","MAGENTA")] = 45;
        bob[SP("MAGENTA","MAGENTA")] = 46;
        bob[SP("CYAN","MAGENTA")] = 47;
        bob[SP("WHITE","MAGENTA")] = 48;
        bob[SP("BLACK","CYAN")] = 49;
        bob[SP("RED","CYAN")] = 50;
        bob[SP("GREEN","CYAN")] = 51;
        bob[SP("YELLOW","CYAN")] = 52;
        bob[SP("BLUE","CYAN")] = 53;
        bob[SP("MAGENTA","CYAN")] = 54;
        bob[SP("CYAN","CYAN")] = 55;
        bob[SP("WHITE","CYAN")] = 56;
        bob[SP("BLACK","WHITE")] = 57;
        bob[SP("RED","WHITE")] = 58;
        bob[SP("GREEN","WHITE")] = 59;
        bob[SP("YELLOW","WHITE")] = 60;
        bob[SP("BLUE","WHITE")] = 61;
        bob[SP("MAGENTA","WHITE")] = 62;
        bob[SP("CYAN","WHITE")] = 63;
        bob[SP("WHITE","WHITE")] = 64;
    }

    SP p(fore,back);

    MT::const_iterator cit = bob.find(p);

    if (bob.end() == cit)
        return 0;

    return (*cit).second;
}

unsigned long Color::pair(const std::string& fore,
                          const std::string& back,
                          const std::string& attr)
{
    if (!(hasColors))
        return 0;

    short num = byName(fore, back);

    if (num == 0)
        return 0;

    unsigned long pair = COLOR_PAIR(num);

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


