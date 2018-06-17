#include <Engine/Flow/StateManager.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Config/Globals.hpp>
#include <Config/Arguments.hpp>
#include <States/GameStateMainMenu.hpp>

#include <locale.h>
#include <libintl.h>
#include <iostream>
#include <cstdlib>


int main(int argc, char *argv[])
{
/*	char* cwd = getenv("PWD");
    std::cout << "getenv(PWD): " << (cwd?cwd:"NULL") << std::endl;
    char* l = getenv("LANG");
    std::cout << "getenv(LANG): " << (l?l:"NULL") << std::endl;
    char* s = setlocale(LC_ALL, "");
    std::cout << "setlocale(): " << (s?s:"NULL") << std::endl;
    std::cout << "bindtextdomain(): " << bindtextdomain("nSnakeGt", cwd) << std::endl;
    std::cout << "textdomain(): " << textdomain( "nSnakeGt") << std::endl;

	setlocale( LC_ALL, "es_ES" );//Spanish
	bindtextdomain( "nSnakeGt", "." );// set directory containing message catalogs 
	textdomain( "nSnakeGt" );//set domain for future gettext() calls
*/
	 setlocale (LC_ALL, "");
     bindtextdomain ("hello", "/usr/share/locale/");
     textdomain ("hello");

	printf(gettext("Hello World\n"));

	
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

