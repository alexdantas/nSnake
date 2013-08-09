#ifndef COLOR_H_DEFINED
#define COLOR_H_DEFINED

#include <string>

/// Colors to be used on the terminal.
///
/// The functions in this file are related to curses attributes.
/// We attempt to provide a friendlier interface for client apps
/// which wantto manipulate attributes, particularly by name.
///
/// A lot was taken from ncutil.
/// Thanks for the help, man! you did an awesome job.
///
class Color
{
public:

    // Possible engine color pairs (FOREGROUND_BACKGROUND).
    // This defines all possible combinations of color pairs
    // on ncurses.
    // Note that the order of definition is important, also
    // the intial value of 1.
    //
    // To access a specific color pair, use this formula:
    //   pair(n) = (foreground * 8) + background
    //
    // Just like
    //   pair    = (COLOR_RED * 8) + COLOR_BLUE
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

    /// Starts all support for colors
    //
    //  Sets up a full 64-color palette.
    //  Note that NCursesWindow uses the global functions
    //  and stdscr for setting up colors, so the colors set
    //  up here affect all linked-in curses code which uses
    //  COLOR_PAIR(number).
    //
    //  For each BG/FG combination of the following colors:
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
    // <pre>
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
    // </pre>
    static bool init();

    /// Returns the color pair according to it's names.
    static unsigned long pair(const std::string& fore,
                              const std::string& back,
                              const std::string& attr="");

private:
    static bool hasColors;
    static bool canChangeColors;
};

/****

     color_pairnum_for_names() accepts to string-form color names, forground
     and background color, and tries to find a matching COLOR_PAIR number.
     It returns 0 if it does not find a match. The color names are
     case-insensitive, and must be one of:


     BLACK
     RED
     GREEN
     YELLOW
     BLUE
     MAGENTA
     CYAN
     WHITE


     You can get the curses attribute values from the return value by passing
     it to the curses COLOR_PAIR(pairnumber) macro.

****/

short color_pairnum_for_names( const std::string & fore, const std::string & back );

/**
   unsigned long color_pair() returns the same as COLOR_PAIR(color_pairnum_for_names(fore,back)),
   or returns 0 if no such pair is found.
*/
unsigned long color_pair( const std::string & fore, const std::string & back );

#endif /* COLOR_H_DEFINED */

