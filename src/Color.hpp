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

    /// Possible engine color pairs (FOREGROUND_BACKGROUND).
    ///
    /// This defines all possible combinations of color pairs
    /// on ncurses.
    ///
    /// ## For developers
    ///
    /// The order of definition is important, also is
    /// the intial value of 1.
    ///
    /// To access a specific color pair, use this formula:
    ///     pair(n) = (foreground * 8) + background
    ///
    /// Just like
    ///     pair    = (COLOR_RED * 8) + COLOR_BLUE
    enum Colors
    {
        BLACK_BLACK = 1, RED_BLACK,   GREEN_BLACK,   YELLOW_BLACK,   BLUE_BLACK,   MAGENTA_BLACK,   CYAN_BLACK,   WHITE_BLACK,
        BLACK_RED,       RED_RED,     GREEN_RED,     YELLOW_RED,     BLUE_RED,     MAGENTA_RED,     CYAN_RED,     WHITE_RED,
        BLACK_GREEN,     RED_GREEN,   GREEN_GREEN,   YELLOW_GREEN,   BLUE_GREEN,   MAGENTA_GREEN,   CYAN_GREEN,   WHITE_GREEN,
        BLACK_YELLOW,    RED_YELLOW,  GREEN_YELLOW,  YELLOW_YELLOW,  BLUE_YELLOW,  MAGENTA_YELLOW,  CYAN_YELLOW,  WHITE_YELLOW,
        BLACK_BLUE,      RED_BLUE,    GREEN_BLUE,    YELLOW_BLUE,    BLUE_BLUE,    MAGENTA_BLUE,    CYAN_BLUE,    WHITE_BLUE,
        BLACK_MAGENTA,   RED_MAGENTA, GREEN_MAGENTA, YELLOW_MAGENTA, BLUE_MAGENTA, MAGENTA_MAGENTA, CYAN_MAGENTA, WHITE_MAGENTA,
        BLACK_CYAN,      RED_CYAN,    GREEN_CYAN,    YELLOW_CYAN,    BLUE_CYAN,    MAGENTA_CYAN,    CYAN_CYAN,    WHITE_CYAN,
        BLACK_WHITE,     RED_WHITE,   GREEN_WHITE,   YELLOW_WHITE,   BLUE_WHITE,   MAGENTA_WHITE,   CYAN_WHITE,   WHITE_WHITE,

        // The following color pairs are not supported on all terminals.
        //
        // But when they are, the background is the terminal's custom
        // background, as defined by the user before running the program.
        //
        // They're initialized on Color::init() and can be called on
        // Color::pair() with the "default" attribute.
        //
        // If default backgrounds are not supported by the terminal, it
        // will fall back to BLACK.
        BLACK_DEFAULT,   RED_DEFAULT, GREEN_DEFAULT, YELLOW_DEFAULT, BLUE_DEFAULT, MAGENTA_DEFAULT, CYAN_DEFAULT, WHITE_DEFAULT
    };

    /// Starts all support for colors, setting up a full
    /// 64-color palette.
    ///
    static bool init();

    /// Returns a color pair according to it's names and attributes.
    ///
    /// If the color is invalid, returns 0.
    /// Color names are case insensitive and can be:
    ///
    ///     black, red, green, yellow, blue, magenta, cyan, white
    ///
    /// Attributes are case sensitive and can be:
    ///
    ///     bold, underline, reverse, blink
    ///
    /// For example:
    ///
    ///     unsigned long c = Color::pair("blue", "black", "bold");
    ///
    ///     c = Color::pair("yellow", "magenta", "underline");
    ///
    static unsigned long pair(const std::string& fore,
                              const std::string& back="default",
                              const std::string& attr="");

private:
    /// Tells if we can use colors on the current terminal.
    static bool hasColors;

    /// Tells if we can have (up to 255) custom colors on the current
    /// terminal.
    static bool canChangeColors;

    /// Tells if we can use the background this terminal had before
    /// running the program (whatever color it might be).
    static bool hasDefaultBackground;
};

#endif /* COLOR_H_DEFINED */

