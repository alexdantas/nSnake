#ifndef WINDOWGAMEHELP_H_DEFINED
#define WINDOWGAMEHELP_H_DEFINED

#include <Engine/Graphics/Window.hpp>

#include <vector>

/// Specific Window that shows Help and other info
/// during Game.
///
class WindowGameHelp
{
public:
	WindowGameHelp();
	virtual ~WindowGameHelp() {};

	/// Updates and draws all tabs.
	void run();

private:
	Window* main;
	std::vector<Window*> windows;
};

#endif //WINDOWGAMEHELP_H_DEFINED

