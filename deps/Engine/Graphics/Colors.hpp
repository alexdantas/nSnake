#ifndef COLORS_H_DEFINED
#define COLORS_H_DEFINED

#include <ncurses.h>
#include <string>

/// Single color.
/// Pretty much useless, only used with `ColorPair`.
typedef short  Color;

/// Color pair (foreground and background).
/// Each character can get shown with a `ColorPair`.
typedef chtype ColorPair;

/// Default color of the current terminal.
/// Means the default
#define COLOR_DEFAULT -1

/// Defines colors to display characters on the screen.
///
namespace Colors
{
	/// Tells if this terminal has support for colors.
	/// Big news: most likely it does.
	///
	/// @note It only gets updated after calling `Colors::init()`
	///
	extern bool hasColors;

	/// Initialize support for colors on the terminal.
	bool init();

	/// Returns a new color with red green and blue components.
	///
	/// @note They go from 0 (none) to 255 (full)
	///
	Color rgb(short red, short green, short blue);

	/// Returns a new color with hex notation.
	///
	/// For example:
	/// - "#ff0000" is red
	/// - "#ff00ff" is magenta.
	///
	Color hex(std::string hex);

	/// Returns a new color from a human-readable string.
	/// For example: "red", "yellow" or "blue"
	///
	/// @note It is very limited for now, only having the 8
	///       basic ncurses colors.
	Color fromString(std::string str);

	/// Returns a new `ColorPair` made with #foreground and #background colors.
	///
	/// @note #is_bold usually brightens the color.
	///
	ColorPair pair(Color foreground, Color background, bool is_bold=false);

	/// Returns a new `ColorPair` made with human-readable strings.
	///
	/// @see Colors::pair()
	/// @see Colors::fromString()
	///
	ColorPair pairFromString(std::string foreground, std::string background, bool is_bold=false);

	/// Activates the colors #foreground and #background on a ncurses #window.
	///
	/// It has the effect of making all subsequent prints having this
	/// color pair.
	///
	void activate(WINDOW* window, Color foreground, Color background);

	/// Activates the color pair #color on a ncurses #window.
	///
	/// @see Colors::activate()
	///
	void pairActivate(WINDOW* window, ColorPair color);
}

#endif //COLORS_H_DEFINED

