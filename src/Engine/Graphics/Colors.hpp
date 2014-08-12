#ifndef COLORS_H_DEFINED
#define COLORS_H_DEFINED

#include <ncurses.h>
#include <string>

typedef short  Color;
typedef chtype ColorPair;

#define COLOR_DEFAULT -1

///
namespace Colors
{
	bool init();

	Color rgb(short r, short g, short b);

	Color hex(std::string hex);

	ColorPair pair(Color foreground, Color background, bool is_bold=false);

	Color fromString(std::string str);

	ColorPair pairFromString(std::string foreground, std::string background, bool is_bold=false);

	void activate(WINDOW* window, Color foreground, Color background);

	void pairActivate(WINDOW* window, ColorPair color);

	// Var
	extern bool hasColors;
};

#endif //COLORS_H_DEFINED

