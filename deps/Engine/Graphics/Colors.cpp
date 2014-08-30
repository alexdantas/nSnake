#include <Engine/Graphics/Colors.hpp>

#include <cstdlib>				// strtol()

// Single Color

Color::Color(std::string name):
	name(name),
	red(-1),
	green(-1),
	blue(-1),
	ncurses_color(COLOR_WHITE)
{ }

std::string Color::toString()
{
	if (! this->name.empty())
		return this->name;

	return (Utils::String::toString(this->red) + "," +
	        Utils::String::toString(this->green) + "," +
	        Utils::String::toString(this->blue));
}

Color Color::fromString(std::string str)
{
	if (str.find(",") == std::string::npos)
	{
		// Format is "name"
		return Colors::name(str);
	}
	else
	{
		// Format "red,green,blue"
		std::vector<std::string> v = Utils::String::split(str, ',');
		if (v.size() != 3)
			return Color();

		for (size_t i = 0; i < v.size(); ++i)
			v[i] = Utils::String::trim(v[i]);

		int red   = Utils::String::to<int>(v[0]);
		int green = Utils::String::to<int>(v[1]);
		int blue  = Utils::String::to<int>(v[2]);

		return Colors::rgb(red, green, blue);
	}
}


// Color Pair


ColorPair::ColorPair():
	bold(false),
	ncurses_pair(0)
{ }

ColorPair::ColorPair(Color foreground, Color background):
	foreground(foreground),
	background(background),
	bold(false),
	ncurses_pair(0)
{ }

std::string ColorPair::toString()
{
	std::string bold = (this->bold ?
	                    "!" :
	                    "");

	return (foreground.toString() + "+" + background.toString() + bold);
}

ColorPair ColorPair::fromString(std::string str)
{
	bool is_bold = false;

	size_t pos = str.find("!");
	if (pos != std::string::npos)
	{
		is_bold = true;
		str.erase(pos, 1);
	}

	std::vector<std::string> v = Utils::String::split(str, '+');

	if (v.size() != 2)
		return ColorPair();

	Color fg = Color::fromString(v[0]);
	Color bg = Color::fromString(v[1]);

	return Colors::pair(fg, bg, is_bold);
}

// All Colors

/// Default color of the current terminal.
///
/// This is the color your terminal has - both for
/// foreground and background.
#define COLOR_DEFAULT -1

bool Colors::hasColors = false;

bool Colors::init()
{
	if (has_colors() != TRUE)	/* ncurses BOOL */
	{
		Colors::hasColors = false;
		return false;
	}
	Colors::hasColors = true;

	start_color();

	// This is a big hack to initialize all 64
	// possible color pairs in ncurses.
	//
	// The thing is, all colors are between
	// COLOR_BLACK and COLOR_WHITE.
	// Since I've set a large number of enums covering
	// all possibilities, I can do it all in a for loop.
	// Check 'man init_pair' for more details.
	//
	// Here's the internal value of colors,
	// taken straight from <curses.h>:
	//
	// #define COLOR_BLACK   0
	// #define COLOR_RED     1
	// #define COLOR_GREEN   2
	// #define COLOR_YELLOW  3
	// #define COLOR_BLUE    4
	// #define COLOR_MAGENTA 5
	// #define COLOR_CYAN    6
	// #define COLOR_WHITE   7
	//
	int i, j, k = 1;
	for (i = COLOR_BLACK; i <= COLOR_WHITE; i++)
	{
		for (j = COLOR_BLACK; j <= COLOR_WHITE; j++)
		{
			init_pair(k, i, j);
			k++;
		}
	}

	// Besides the normal color pairs, we can use
	// whatever colors the user has currently set to
	// their terminal.
	// It looks more "natural".
	//
	// So COLOR_PAIR(-1, -1) is the default foreground
	// and background.
	//
	// Let's do it if the current terminal supports it.

	if (use_default_colors() != ERR)
	{
		// default background
		init_pair(64, COLOR_BLACK,   COLOR_DEFAULT);
		init_pair(65, COLOR_RED,     COLOR_DEFAULT);
		init_pair(66, COLOR_GREEN,   COLOR_DEFAULT);
		init_pair(67, COLOR_YELLOW,  COLOR_DEFAULT);
		init_pair(68, COLOR_BLUE,    COLOR_DEFAULT);
		init_pair(69, COLOR_MAGENTA, COLOR_DEFAULT);
		init_pair(70, COLOR_CYAN,    COLOR_DEFAULT);
		init_pair(71, COLOR_WHITE,   COLOR_DEFAULT);
	}
	return true;
}

Color Colors::rgb(short red, short green, short blue)
{
	Color color;
	color.name = ""; // since it's RGB

	if ((can_change_color() == FALSE) ||
	    (COLORS < 256))
		return color;

	static int color_no = 8;
	color_no++;
	if (color_no >= COLORS)
		color_no = 8;

	// `init_color` receives values from 0 to 1000,
	// so we'll map from 255 to 1000
	int expand = 1000/255;

	init_color((color_no - 1), red*expand, green*expand, blue*expand);
	color.ncurses_color = (color_no - 1);

	color.red   = red;
	color.green = green;
	color.blue  = blue;
	return color;
}
Color Colors::hex(std::string hex)
{
	Color color;

	if ((hex[0]     != '#') || // sorry
	    (hex.size() != 7)) // #RRGGBB format
		return color;

	char col[3];
	col[2] = '\0';

	col[0] = hex[1];
	col[1] = hex[2];
	long r = strtol(col, NULL, 16);

	col[0] = hex[3];
	col[1] = hex[4];
	long g = strtol(col, NULL, 16);

	col[0] = hex[5];
	col[1] = hex[6];
	long b = strtol(col, NULL, 16);

	return Colors::rgb(r, g, b);
}

ColorPair Colors::pair(Color& foreground, Color& background, bool is_bold)
{
	ColorPair pair(foreground, background);
	if (is_bold)
		pair.bold = true;

	short fg = foreground.ncurses_color;
	short bg = background.ncurses_color;

	// Basic nCurses colors
	if ((fg < 8) && (bg < 8))
	{
		if (bg == COLOR_DEFAULT)
		{
			if (is_bold)
				pair.ncurses_pair = COLOR_PAIR(64 + fg) | A_BOLD;
			else
				pair.ncurses_pair = COLOR_PAIR(64 + fg);

			return pair;
		}

		if (is_bold)
			pair.ncurses_pair = COLOR_PAIR(fg*8 + bg + 1) | A_BOLD;
		else
			pair.ncurses_pair = COLOR_PAIR(fg*8 + bg + 1);

		return pair;
	}

	if (COLORS < 256)
	{
		if (is_bold)
			pair.ncurses_pair = COLOR_PAIR(0) | A_BOLD;
		else
			pair.ncurses_pair = COLOR_PAIR(0);

		return pair;
	}

	// Will create color pair
	// (above the 64 regular ones plus 12 default = 72)
	static int color_pair_no = 72;
	color_pair_no++;
	if (color_pair_no >= COLOR_PAIRS)
		color_pair_no = 72;

	init_pair((color_pair_no - 1), fg, bg);

	if (is_bold)
		pair.ncurses_pair = COLOR_PAIR(color_pair_no - 1) | A_BOLD;
	else
		pair.ncurses_pair = COLOR_PAIR(color_pair_no - 1);

	return pair;
}
Color Colors::name(std::string str)
{
	Color color;

	if (str.empty())
		return color;

	     if (str == "default") color.ncurses_color = COLOR_DEFAULT;
	else if (str == "black")   color.ncurses_color = COLOR_BLACK;
	else if (str == "red")     color.ncurses_color = COLOR_RED;
	else if (str == "green")   color.ncurses_color = COLOR_GREEN;
	else if (str == "yellow")  color.ncurses_color = COLOR_YELLOW;
	else if (str == "blue")    color.ncurses_color = COLOR_BLUE;
	else if (str == "magenta") color.ncurses_color = COLOR_MAGENTA;
	else if (str == "cyan")    color.ncurses_color = COLOR_CYAN;
	else if (str == "white")   color.ncurses_color = COLOR_WHITE;

	color.name = str;
	return color;
}

ColorPair Colors::pair(std::string foreground, std::string background, bool is_bold)
{
	if (foreground.empty() || background.empty())
		return ColorPair();

	Color f = Colors::name(foreground);
	Color b = Colors::name(background);

	return Colors::pair(f, b, is_bold);
}

void Colors::activate(WINDOW* window, Color& foreground, Color& background)
{
	ColorPair pair = Colors::pair(foreground, background);

	Colors::pairActivate(window, pair);
}

void Colors::pairActivate(WINDOW* window, ColorPair& color)
{
	wattrset(window, color.ncurses_pair);
}

