#ifndef WINDOW_H_DEFINED
#define WINDOW_H_DEFINED

#include <string>
#include <vector>
#include <ncurses.h>

#include <Engine/Graphics/Colors.hpp>

/// Handy alias to make child Windows stretch their sizes to
/// the maximum allowed by the parent.
#define WINDOW_FILL 0

/// A segment of the terminal screen (2D char matrix).
///
class Window
{
public:
	enum BorderType
	{
		BORDER_NONE, BORDER_REGULAR, BORDER_FANCY
	};

	Window(int x, int y, int w, int h);

	Window(Window* parent, int x, int y, int width, int height);

	virtual ~Window();

	virtual void resize(int w, int h);

	/// Shows text #str at #x #y on the window with color #pair.
	///
	/// @note It defaults to white text on black background.
	void print(std::string str, int x, int y, ColorPair pair=ColorPair());

	/// Shows multiple text lines #lines at #x #y on the window
	/// with color #pair.
	///
	/// @note It also defaults to white text on black background.
	/// @note Use it together with `Utils::String::split`:
	///
	///     window.print(Utils::String::split("multiple\nlines", '\n'),
	///                 x, y, color_pair);
	///
	void print(std::vector<std::string> lines, int x, int y, ColorPair pair=ColorPair());

	/// Shows #c at #x #y with color #pair.
	///
	/// @note It also defaults to white text on black background.
	void printChar(int c, int x, int y, ColorPair pair=ColorPair());

	void setBackground(chtype ch, ColorPair pair);

	void refresh();

	void clear();

	int getW() const;
	int getH() const;
	int getX() const;
	int getY() const;

	/// Applies border of #type to this Window.
	void borders(BorderType type);

	/// Applies a board style dependent on the
	/// EngineGlobals settings
	void setBorders();

	void horizontalLine(int x, int y, int c, int width, ColorPair pair);

	/// All possible positions where a Window title can be.
	/// Note that a Window can have titles on all of those.
	enum WindowTitlePosition
	{
		TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT
	};

	/// Sets a text that will appear at the top of the Window.
	///
	/// By default it's shown on top of the borders, with
	/// a highlighted color tone.
	///
	void setTitle(std::string title, WindowTitlePosition position=Window::TOP_LEFT);

protected:

	/// Ncurses' internal data structure.
	/// Don't mess with it.
	WINDOW* win;

	int x;
	int y;
	int width;
	int height;

	BorderType borderType;

	std::string topLeftTitle;
	std::string topRightTitle;
	std::string bottomLeftTitle;
	std::string bottomRightTitle;
};

#endif //WINDOW_H_DEFINED

