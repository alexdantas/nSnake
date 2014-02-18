#ifndef WINDOW_H_DEFINED
#define WINDOW_H_DEFINED

#include <string>
#include <ncurses.h>

#include <Interface/Colors.hpp>

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

	bool isValid();

	/// Shows text #str at #x #y on the window with color #pair.
	///
	/// @note It defaults to white text on black background.
	void print(std::string str, int x, int y, ColorPair pair=0);

	/// Shows text #str at #x #y on the window with color #pair.
	/// The difference is that we respect newlines, showing
	/// it up at the next line just below text above.
	///
	/// @note It also defaults to white text on black background.
	void print_multiline(std::string str, int x, int y, ColorPair pair=0);

	/// Shows #c at #x #y with color #pair.
	///
	/// @note It also defaults to white text on black background.
	void printChar(int c, int x, int y, ColorPair pair=0);

	void setBackground(chtype ch, ColorPair pair);

	void refresh();

	void clear();

	int getW() const;
	int getH() const;
	int getX() const;
	int getY() const;

	void borders(BorderType type);

	void horizontalLine(int x, int y, int c, int width, ColorPair pair);

	void setTitle(std::string title);


	/// Ncurses' internal data structure.
	/// IT'S PUBLIC BECAUSE SOMETIMES I NEED TO CALL DIRECT
	/// NCURSES CALLS - FIX THIS
	WINDOW* win;

protected:
	/// Tells if we've got some initialization error.
	bool error;

	int x;
	int y;
	int width;
	int height;

	BorderType borderType;

	std::string title;
};

#endif //WINDOW_H_DEFINED

