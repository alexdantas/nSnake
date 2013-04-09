/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * nSnake - The classic snake game with ncurses.                              *
 * Copyright (C) 2011-2012  Alexandre Dantas (kure)                           *
 *                                                                            *
 * This file is part of nSnake.                                               *
 *                                                                            *
 * nSnake is free software: you can redistribute it and/or modify             *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation, either version 3 of the License, or          *
 * any later version.                                                         *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                            *
 * homepage: http://sourceforge.net/projects/nsnake/                          *
 *                                                                            *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/** @file arguments.c
 *
 *  Defines the procedures related to handling the command-line arguments.
 *  Version 1.0 (31/12/11)
 */


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>     /* For getopt_long() */

#include "arguments.h"
#include "hscores.h"


/** Handles all the commandline arguments.
 *
 * This is the main function of this module. It processes
 * an argv[] array of argc elements based on the options
 * specified at the struct option.
 *
 * If you want to add another argument, follow these steps:
 * # Add an option on the struct option.
 * # Add the short equivalent (if it exists) on the getopt_long() call.
 * # Add a case on the switch specifying the action of that option.
 */
void args_handle (int argc, char* argv[])
{
	static struct option options[] =
	{
		{"help",         no_argument, NULL, 'h'},
		{"license",      no_argument, NULL, 'l'},
		{"version",      no_argument, NULL, 'v'},
		{"reset-scores", no_argument, NULL, 'r'},
		/* The last element must be all zeroes */
		{0, 0, 0, 0}
	};
	/* The index of the current option */
	int option_index;
	/* The character for comparison */
	int c = 0;


	/* We keep checking the arguments untill they run out (c == -1) */
	while (c != -1)
	{
		c = getopt_long (argc, argv, "hlvr", options, &option_index);

		switch (c)
		{
		case 'h':
			print_help ();
			exit (EXIT_SUCCESS);
			break;

		case 'l':
			print_license ();
			exit (EXIT_SUCCESS);
			break;

		case 'v':
			print_version ();
			exit (EXIT_SUCCESS);
			break;

		case 'r':
			// TODO TODO TODO TODO TODO TODO TODO TODO RESET SCORES
			hscore_clean ();
			printf ("* HighScores reseted\n");
			exit (EXIT_SUCCESS);
			break;

		case '?':
			/* getopt_long() already printed an error message about
			 * unrecognized option */
			print_usage ();
			exit (EXIT_FAILURE);
			break;

		case -1:
			// There were no '-' parameters passed
			// or all the parameters were processed
			break;

		default:
			print_usage ();
			exit (EXIT_FAILURE);
			break;
		}
	}

	/* Just in case the user specified more arguments (not options)
	 * than needed, you decide what to do. Here, we just ignore them */
	while (optind < argc)
		optind++;
}


/**	Prints Help instructions on standard output.
 */
void print_help ()
{
	printf("nSnake Help\n");
	printf("\n");
	printf("Synopsis:\n");
	printf("\tThe classic snake game. You control a snake pursuing\n");
	printf("\tASCII-like fruits.\n");
	printf("Controls:\n");
	printf("\tNumbers (1~9)     Changes the game speed at the main menu\n");
	printf("\tArrow Keys, WASD  Control the snake directions\n");
	printf("\tq                 Quits the game at any time\n");
	printf("\tp                 Pauses/Unpauses the game\n");
	printf("Usage:\n");
	printf("\tnsnake [-h] [-l] [-v] [-r]\n");
	printf("Commandline arguments:\n\n");
	printf("\t-h, --help         Displays the help guidelines.\n");
	printf("\t-l, --license      Displays this program's license and warranty.\n");
	printf("\t-v, --version      Displays the version and general information.\n");
	printf("\t-r, --reset-scores Resets all the Highscores to default.\n");
	printf("Type 'man nsnake' for more information.\n");
	printf("\n");
}


/**	Prints the preamble of the GNU GPL license v3 on standard output.
 */
void print_license ()
{
	printf("nSnake - The classic snake game with ncurses.\n");
	printf("Copyright (C) 2011-2012  Alexandre Dantas (kure)\n");
	printf("\n");
	printf("nSnake is free software: you can redistribute it and/or modify\n");
	printf("it under the terms of the GNU General Public License as published by\n");
	printf("the Free Software Foundation, either version 3 of the License, or\n");
	printf("any later version.\n");
	printf("\n");
	printf("This program is distributed in the hope that it will be useful,\n");
	printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	printf("GNU General Public License for more details.\n");
	printf("\n");
	printf("You should have received a copy of the GNU General Public License\n");
	printf("along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
	printf("\n");
}


/**	Prints the program usage on standard output.
 */
void print_usage ()
{
	printf("nSnake v"VERSION" Usage:\n");
	printf("\tnsnake [-h] [-l] [-v] [-r]\n");
	printf("Commandline arguments:\n\n");
	printf("\t-h, --help         Displays the help guidelines.\n");
	printf("\t-l, --license      Displays this program's license and warranty.\n");
	printf("\t-v, --version      Displays the version and general information.\n");
	printf("\t-r, --reset-scores Resets all the Highscores to default.\n");
	printf("\n");
}


/**	Prints the program version on standard output.
 */
void print_version ()
{
	printf("nSnake v"VERSION"\t("DATE")\n");
	printf("Copyright (C) 2011-2012  Alexandre Dantas (kure)\n");
	printf("\n");
	printf("This program comes with ABSOLUTELY NO WARRANTY\n");
	printf("for warranty details type 'nsnake --license'.\n");
	printf("This is free software, and you are welcome to redistribute it\n");
	printf("under certain conditions; type 'nsnake --license' for license details.\n");
	printf("\n");
	printf("Mailto:   alex.dantas92@gmail.com\n");
	printf("Homepage: http://sourceforge.net/projects/nsnake\n");
	printf("\n");
}
