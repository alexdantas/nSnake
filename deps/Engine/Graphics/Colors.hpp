#ifndef COLORS_H_DEFINED
#define COLORS_H_DEFINED

#include <Engine/Helpers/Utils.hpp>

#include <ncurses.h>
#include <string>
#include <vector>

/// Single color.
/// Pretty much useless, only used with `ColorPair`.
struct Color
{
	std::string name; // if it's empty, means use RGB
	int red; // if it's -1, means use the name
	int green;
	int blue;

	short ncurses_color;

	Color():
		name("white"),
		red(-1),
		green(-1),
		blue(-1),
		ncurses_color(COLOR_WHITE)
	{ }

	std::string toString()
	{
		if (! this->name.empty())
			return this->name;

		return (Utils::String::toString(this->red) + "," +
		        Utils::String::toString(this->green) + "," +
		        Utils::String::toString(this->blue));
	}
	void fromString(std::string str)
	{
		if (str.find(",") == std::string::npos)
		{
			// Format "name"
			this->name = str;

			this->red   = -1;
			this->green = -1;
			this->blue  = -1;
		}
		else
		{
			// Format "red,green,blue"
			std::vector<std::string> v = Utils::String::split(str, ',');

			if (v.size() != 3)
				return;

			this->red   = Utils::String::to<int>(v[0]);
			this->green = Utils::String::to<int>(v[1]);
			this->blue  = Utils::String::to<int>(v[2]);

			this->name = "";
		}
	}
};

/// Color pair (foreground and background).
/// Each character can get shown with a `ColorPair`.
struct ColorPair
{
	Color foreground;
	Color background;

	chtype ncurses_pair;

	ColorPair():
		ncurses_pair(0)
	{ }

	ColorPair(Color foreground, Color background):
		foreground(foreground),
		background(background),
		ncurses_pair(0)
	{ }
	std::string toString()
	{
		return (foreground.toString() +
		        "+" +
		        background.toString());
	}
	void fromString(std::string str)
	{
		std::vector<std::string> v = Utils::String::split(str, '+');

		if (v.size() != 2)
			return;

		this->foreground.fromString(v[0]);
		this->background.fromString(v[1]);
	}
};

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

	/// Returns a new `ColorPair` made with #foreground and #background colors.
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

	/// Activates the colors #foreground and #background on a ncurses #window.
	///
	/// It has the effect of making all subsequent prints having this
	/// color pair.
	///
	void activate(WINDOW* window, Color& foreground, Color& background);

	/// Activates the color pair #color on a ncurses #window.
	///
	/// @see Colors::activate()
	///
	void pairActivate(WINDOW* window, ColorPair& color);
}

#endif //COLORS_H_DEFINED

