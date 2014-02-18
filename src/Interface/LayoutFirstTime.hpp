#ifndef LAYOUTFIRSTTIME_H_DEFINED
#define LAYOUTFIRSTTIME_H_DEFINED

#include <Interface/Layout.hpp>
#include <Interface/Window.hpp>
#include <Interface/Menu/Menu.hpp>

/// How we show the screen at GameStateFirstTime.
/// See it's documentation there.
class LayoutFirstTime: public Layout
{
public:
	LayoutFirstTime(int width, int height);
	virtual ~LayoutFirstTime();

	void windowsInit();
	void windowsExit();

	void draw();
};

#endif //LAYOUTFIRSTTIME_H_DEFINED

