#include <Flow/StateManager.hpp>
#include <Interface/Ncurses.hpp>
#include <Interface/Colors.hpp>
#include <Config/Globals.hpp>
#include <Config/Arguments.hpp>
#include <Misc/Utils.hpp>

int main(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	try
	{
		Globals::init();
		Arguments::parse(argc, argv);
		Utils::Random::seed();
		Ncurses::init();
		Colors::init();

		StateManager states;
		states.run();

		Ncurses::exit();
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

