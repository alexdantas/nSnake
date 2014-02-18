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
		"yetris v" VERSION " (" DATE ")\n"
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
		"Homepage: http://yetris.alexdantas.net/\n"
		"Contact:  Alexandre Dantas <eu@alexdantas.net>\n";

	command_free(self);
	exit(EXIT_SUCCESS);
}
void help(command_t* self)
{
	(void)(self);

	std::cout <<
		" __ __   ___ ______ ____  ____ _____\n"
		"|  T  T /  _|      |    \\l    / ___/\n"
		"|  |  |/  [_|      |  D  )|  (   \\_ \n"
		"|  ~  Y    _l_j  l_|    / |  |\\__  T\n"
		"l___, |   [_  |  | |    \\ |  |/  \\ |\n"
		"|     |     T |  | |  .  Yj  l\\    |\n"
		"l____/l_____j l__j l__j\\_|____j\\___j\n"
		"v" VERSION "               (built " DATE ")\n"
		"\n"
		"yetris is a customizable Tetris(tm) clone for the\n"
		"terminal.\n"
		"It's settings and scores are stored at:\n"
		"	~/.local/share/yetris/\n"
		"\n"
		"Usage:\n"
		"	yetris [options]\n"
		"\n"
		"	-h, --help     Show this message\n"
		"	-v, --version  Show game version and contact info\n"
		"\n"
		"Thank you for playing this game :)\n";

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

