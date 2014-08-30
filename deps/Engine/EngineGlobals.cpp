#include <Engine/EngineGlobals.hpp>

// Defining `extern` variables
bool EngineGlobals::Screen::center_horizontally;
bool EngineGlobals::Screen::center_vertically;
bool EngineGlobals::Screen::show_borders;
bool EngineGlobals::Screen::fancy_borders;
bool EngineGlobals::Screen::outer_border;

ColorPair EngineGlobals::Theme::text;
ColorPair EngineGlobals::Theme::hilite_text;
ColorPair EngineGlobals::Theme::textbox;


void EngineGlobals::init()
{
	EngineGlobals::Screen::center_horizontally = true;
	EngineGlobals::Screen::center_vertically   = true;

	EngineGlobals::Screen::show_borders  = true;
	EngineGlobals::Screen::fancy_borders = true;
	EngineGlobals::Screen::outer_border  = true;

	EngineGlobals::Theme::text        = Colors::pair("white", "default", true);
	EngineGlobals::Theme::hilite_text = Colors::pair("cyan", "default", true);
	EngineGlobals::Theme::textbox     = Colors::pair("default", "cyan");
}

