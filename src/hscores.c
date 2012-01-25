/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 * nSnake - The classic snake game with ncurses.                              *
 * Copyright (C) 2011  Alexandre Dantas (kure)                                *
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

/** @file hscores.c
 *
 *	Definition of the functions related to high scores.
 */

//	We also get a define from the Makefile called SCORE_PATH
//	It indicates the whole path to the scores file.
//	By default, it is /var/games/nsnake/high.scores
//	but it depends on the installation directory set by the user

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hscores.h"
#include "player.h"


/** The Default HighScore */
int HIGH_SCORE_DEFAULT = 500;
/** The separate Highscore for the game mode with borders */
int HIGH_SCORE_BORDERS;
/** The separate Highscore for the game mode without borders */
int HIGH_SCORE_BORDERS_OFF;


/** Restore the Highscores to the default.
 *
 *  @note If the Highscores file doesn't exist, it is created.
 */
void hscore_clean ()
{
	FILE* file = fopen (SCORE_PATH, "wb");

	HIGH_SCORE_BORDERS = HIGH_SCORE_DEFAULT;
	HIGH_SCORE_BORDERS_OFF = HIGH_SCORE_DEFAULT;

	if (file != NULL)
	{
		fwrite (&HIGH_SCORE_BORDERS, sizeof (int), 1, file);
		fwrite (&HIGH_SCORE_BORDERS_OFF, sizeof (int), 1, file);
		fclose (file);
	}
}


/**	Creates/reads from the High Score file
 *
 * 	First, we try to open for reading. If it doesn't exist, then we
 *  open it for writing, effectively creating it.
 *
 *  @note The path to the score file is defined by the Makefile.
 *        Currently, it is /var/games/.nsnake.scores
 *	@todo clean this code. Lots of useless ifs and elses.
 */
void hscore_init ()
{
	FILE* file = fopen (SCORE_PATH, "rb");

	game.cant_open_hscore_file = 0;

	if (file == NULL)
		hscore_clean ();

	else
	{
		int items_read;

		items_read = fread (&HIGH_SCORE_BORDERS, sizeof (int), 1, file);
		if (items_read != 1)
			nsnake_abort ("Highscore File I/O error!\nTry cleaning the scores file!");

		items_read = fread (&HIGH_SCORE_BORDERS_OFF, sizeof (int), 1, file);
		if (items_read != 1)
			nsnake_abort ("Highscore File I/O error!\nTry cleaning the scores file!");
	}

	file = fopen (SCORE_PATH, "rb");
	if (file == NULL)
	{
		//This time it really couldnt open the score file
		game.cant_open_hscore_file = 1;
	}
	else
		fclose (file);
}


/**	Records the player High Score
 */
void hscore_store ()
{
	FILE* file = fopen (SCORE_PATH, "r+b");

	if (file == NULL)
		hscore_clean ();

	if (game.mode == BORDERS_ON)
		HIGH_SCORE_BORDERS = snake.score;

	else if (game.mode == BORDERS_OFF)
		HIGH_SCORE_BORDERS_OFF = snake.score;



	if (file == NULL)
	{
		// We really couldn't open the high scores file
		game.cant_open_hscore_file = 1;
	}
	else
	{
		if (game.mode == BORDERS_ON)
			fwrite (&HIGH_SCORE_BORDERS, sizeof (int), 1, file);

		else if (game.mode == BORDERS_OFF)
		{
			fseek (file, sizeof (int), SEEK_SET);
			fwrite (&HIGH_SCORE_BORDERS_OFF, sizeof (int), 1, file);
		}

		fclose (file);
	}
}
