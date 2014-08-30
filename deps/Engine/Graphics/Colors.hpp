#ifndef COLORS_H_DEFINED
#define COLORS_H_DEFINED

#include <Engine/Helpers/Utils.hpp>
#include <Engine/Helpers/String.hpp>

#include <ncurses.h>
#include <string>
#include <vector>

/// Single color.
/// Pretty much useless, only used with `ColorPair`.
struct Color
{
	/// Human-readable Color name.
	/// @note If it's empty, means use RGB values.
	std::string name;

	/// Red component of the color, from 0 to 255.
	/// @note If it's -1, means use the name string.
	int red;

	/// Green component of the color, from 0 to 255.
	/// @note If it's -1, means use the name string.
	int green;

	/// Blue component of the color, from 0 to 255.
	/// @note If it's -1, means use the name string.
	int blue;

	/// Internal value for the Ncurses library.
	/// Don't mess with it.
	short ncurses_color;

	/// Creates a new color from its human-readable name.
	///
	/// @note If you want to create a color from its
	///       RGB values, use `Colors::rgb()`
	Color(std::string name="white");

	/// Returns a string representation of this Color.
	/// @note It can be converted back using `fromString`.
	///
	/// The format is "red,green,blue".
	std::string toString();

	/// Makes a color according to a string
	/// representation of a Color.
	/// @note It must be a value returned from `toString`.
	static Color fromString(std::string str);
};

/// Color pair (foreground and background).
/// Each character can get shown with a `ColorPair`.
struct ColorPair
{
	Color foreground;
	Color background;

	/// Is this color bold?
	/// Meaning is it brighter than its normal counterpart?
	bool bold;

	/// Internal ncurses value for a color pair.
	/// Don't mess with it.
	chtype ncurses_pair;

	/// Creates an empty ColorPair.
	/// @note Defaults to "white" over "black"
	ColorPair();

	/// Creates a ColorPair from both Colors.
	ColorPair(Color foreground, Color background);

	/// Creates a string representation of this ColorPair.
	/// @note Use later with `fromString`.
	///
	/// The format is "foreground+background".
	/// If the color is bold, its has '!' at the end
	/// (like "foregroud+background!").
	std::string toString();

	/// Makes a ColorPair be according to a string
	/// representation returned by `toString()`.
	///
	static ColorPair fromString(std::string str);
};

/// Stuff related to Colors on a global scale.
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
	Color name(std::string str);

	/// Returns a new `ColorPair` made with #foreground
	/// and #background colors.
	///
	/// @note #is_bold usually brightens the color.
	///
	ColorPair pair(Color& foreground, Color& background, bool is_bold=false);

	/// Returns a new `ColorPair` made with human-readable strings.
	///
	/// @see Colors::pair()
	/// @see Colors::fromString()
	///
	ColorPair pair(std::string foreground, std::string background, bool is_bold=false);

	/// Activates the colors #foreground and #background on a
	/// ncurses #window.
	///
	/// It has the effect of making all subsequent prints having
	/// this color pair.
	///
	void activate(WINDOW* window, Color& foreground, Color& background);

	/// Activates the color pair #color on a ncurses #window.
	///
	/// @see Colors::activate()
	///
	void pairActivate(WINDOW* window, ColorPair& color);
}

#endif //COLORS_H_DEFINED

