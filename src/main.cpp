#include <Engine/Flow/StateManager.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Config/Globals.hpp>
#include <Config/Arguments.hpp>
#include <States/GameStateMainMenu.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <libintl.h>
#include <locale.h>
#define _(String) gettext (String)

int main(int argc, char *argv[])
{
	/* Setting the i18n environment */
  	setlocale (LC_ALL, "");
  	bindtextdomain ("nsnake", getenv("PWD"));
  	textdomain ("nsnake");

		/* Example of i18n usage */
  	printf(_("Hello World\n"));

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
