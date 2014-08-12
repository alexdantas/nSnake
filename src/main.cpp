#include <Engine/Flow/StateManager.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Game/Config/Globals.hpp>
#include <Game/Config/Arguments.hpp>
#include <Engine/Helpers/Utils.hpp>

int main(int argc, char *argv[])
{
	try
	{
		// Settings
		Globals::init();
		Globals::loadFile();
		Arguments::parse(argc, argv);

		// Misc Initializations
		Utils::Random::seed();
		Ncurses::init();
		Colors::init();

		// Actually running the game
		StateManager states;
		states.run();

		// Finishing things
		Globals::saveFile();
		Ncurses::exit();
		Globals::exit();
	}
	catch (...)
	{
		// I dont really have a nice exception-handling scheme right
		// now. I must learn how to properly deal with them.
		Ncurses::exit();
		return 666;
	}
	return 0;
}

