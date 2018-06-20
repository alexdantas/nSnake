#include <Engine/Flow/StateManager.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Config/Globals.hpp>
#include <Config/Arguments.hpp>
#include <States/GameStateMainMenu.hpp>

#include <locale.h>
#include <libintl.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[])
{

     setlocale (LC_ALL, "");
     bindtextdomain ("es_AR", "/usr/share/locale/");
     textdomain ("es_AR");

	printf(gettext("Hello World\n"));//only for testing gettext

	
	try
	{
		// Settings
		EngineGlobals::init();
		Globals::init();
		Globals::loadFile();
		Arguments::parse(argc, argv);

		// Misc Initializations
		Utils::Random::seed();
		Ncurses::init();
		Colors::init();

		// Actually running the game
		StateManager states;
		states.run(new GameStateMainMenu());

		// Finishing things
		Globals::saveFile();
		Ncurses::exit();
		Globals::warnErrors();
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

