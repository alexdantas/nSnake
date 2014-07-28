#include <Config/Arguments.hpp>

#include <commander/commander.h> // local files
#include <iostream>
#include <cstdlib>

// Local functions that'll be used as callbacks
// for the right switches.

void version(command_t* self)
{
	(void)(self);

	std::cout <<
		"nsnake v" VERSION " (" DATE ")\n"
		"\n"
		"   ,           ,\n"
		"  /             \\  This game was made with\n"
		" ((__-^^-,-^^-__))  and itself is Free Software,\n"
		"  `-_---' `---_-'   licensed under the GPLv3\n"
		"   `--|o` 'o|--'                 <3\n"
		"      \\  `  /\n"
		"       ): :(\n"
		"       :o_o:\n"
		"         -\n"
		"\n"
		"Homepage:      http://nsnake.alexdantas.net/\n"
		"Source Code:   https://github.com/alexdantas/nsnake/\n"
		"Contact:       Alexandre Dantas <eu@alexdantas.net>\n";

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
		"nsnake is the classical snake game on the terminal\n"
		"\n"
		"Settings and scores are stored at:\n"
		"	`~/.local/share/nsnake/`\n"
		"\n"
		"Usage:\n"
		"	nsnake [options]\n"
		"\n"
		"	-h, --help     Show this message\n"
		"	-v, --version  Show game version and contact info\n"
		"\n"
		"See also `man nsnake`\n"
		"Thanks for playing this game :)\n";

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

