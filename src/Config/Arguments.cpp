#include <Config/Arguments.hpp>

#include <commander/commander.h> // local files
#include <iostream>
#include <cstdlib>
#include <libintl.h>
#include <locale.h>


// Local functions that'll be used as callbacks
// for the right switches.

void version(command_t* self)
{
	(void)(self);

	std::cout <<
		"nsnake v" VERSION " (" DATE ")\n"<<
		"\n"
		"   ,           ,\n"
		//std::cout << gettext("Hello, world!") << "\n";
		<<gettext("  /             \\  This game was made with")<<"\n"
		<<gettext(" ((__-^^-,-^^-__))  and itself is Free Software,")<<"\n"
		<<gettext("  `-_---' `---_-'   licensed under the GPLv3")<<"\n"
		"   `--|o` 'o|--'                 <3\n"
		"      \\  `  /\n"
		"       ): :(\n"
		"       :o_o:\n"
		"         -\n"
		"\n"
		<<gettext("Homepage:      http://nsnake.alexdantas.net/")<<"\n"
		<<gettext ("Source Code:   https://github.com/alexdantas/nsnake/")<<"\n"
		<<gettext ("Contact:       Alexandre Dantas <eu@alexdantas.net>")<<"\n";

	command_free(self);
	exit(EXIT_SUCCESS);
}
void help(command_t* self)
{
	(void)(self);

	std::cout <<
		" _      __   _       __    _     ____ \n"
		"| |\\ | ( (` | |\\ |  / /\\  | |_/ | |_  \n"
		"|_| \\| _)_) |_| \\| /_/--\\ |_| \\ |_|__ \n"
		"v" VERSION "                 (built " DATE ")\n"
		"\n"
		<<gettext("nsnake is the classical snake game on the terminal")<<"\n"
		"\n"
		<<gettext("Settings and scores are stored at:")<<"\n"
		"	`~/.local/share/nsnake/`\n"
		"\n"
		<<gettext("Usage:")<<"\n"
		"	nsnake [options]\n"
		"\n"
		<<gettext("	-h, --help     Show this message")<<"\n"
		<<gettext("	-v, --version  Show game version and contact info")<<"\n"
		"\n"
		<<gettext("See also `man nsnake`")<<"\n"
		<<gettext("Thanks for playing this game :)")<<"\n";

	command_free(self);
	exit(EXIT_SUCCESS);
}

void Arguments::parse(int argc, char* argv[])
{
	// commander internal data structure
	command_t cmd;
	command_init(&cmd, argv[0], VERSION);

	command_option(&cmd, "-v", "--version", "Show game version and build date", version);
	command_option(&cmd, "-h", "--help",    "Show instructions", help);

	command_parse(&cmd, argc, argv);
	command_free(&cmd);
}

