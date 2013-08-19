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

/** @file hscores.c
 *
 *	Definition of the functions related to high scores.
 */

//	We get one define from the Makefile:
//	SCORE_FILE -> Filename of the high score file
//
//	They can be changed by the user there, but the defaul is
//	SCORE_FILE -> high-scores.bin

// If we're on Windows, there's no home dir!
// So we create the file on the same directory as the .exe
#if (defined __WIN32__) && (!defined __CYGWIN__)
	#undef  SCORE_PATH
	#define SCORE_PATH "hscores.bin"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>			/* mkdir(), stat() */
#include <sys/types.h>			/* mkdir(), stat() */
#include <errno.h>				/* errno */

#include "hscores.h"
#include "player.h"


/** The Default HighScore */
int HIGH_SCORE_DEFAULT = 500;

/** The separate Highscore for the game mode with borders */
int HIGH_SCORE_BORDERS = 0;

/** The separate Highscore for the game mode without borders */
int HIGH_SCORE_BORDERS_OFF = 0;

/** The directory where we'll keep the highscore file */
char SCORE_DIR[255];

/** Full absolute path to the highscore file */
char SCORE_PATH[255];

/** Initializes all global variables needed for high scores.
 *	@note Can be called multiple times, won't overwrite things.
 */
int hscore_globals_init()
{
	static int initialized = 0;

	if (initialized)
		return -1;

	initialized = 1;
	game.cant_open_hscore_file = 0;

	memset(SCORE_DIR, '\0', 255);
	memset(SCORE_PATH, '\0', 255);
	if (getenv("HOME") == NULL)
	{
		strncpy(SCORE_DIR,	"/dev",		 254);
		strncpy(SCORE_PATH, "/dev/null", 254);
		game.cant_open_hscore_file = 1;
		return -1;
	}

	strncpy(SCORE_DIR, getenv("HOME"), 254);
	strncat(SCORE_DIR, "/.nsnake", 254);
	strncpy(SCORE_PATH, SCORE_DIR, 254);
	strncat(SCORE_PATH, "/", 1);
	strncat(SCORE_PATH, SCORE_FILE, 254);
	return 0;
}

/** Local function that tests if the directory #path exists.
 *	@note Must be a null-terminated absolute path.
 *	@return 1 if directory exists, 0 if it doesn't.
 */
int directory_exists(char* path)
{
	struct stat s;
	int err = stat(path, &s);
	if (err == -1)
	{
		if (errno == ENOENT)	/* doesn't exist */
			return 0;
	}
//	else
//	{
//		  if (S_ISDIR(s.st_mode))
//			// exists and it's a directory
//		  else
//			  // exists but it's not a directory
//	  }
	return 1;
}

/** Creates a directory on #path.
 *	@return -1 on error, 0 on success.
 *	@note Creates with permissions 755.
 */
int create_directory(char* path)
{
	return mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH);
}

/** Check if a file exists.
 *	@return 0 if doesnt exist, 1 if exists
 */
int file_exists(char* path)
{
	struct stat s;
	int err = stat(path, &s);
	if (err == -1)
	{
		if (errno == ENOENT)	/* doesn't exist */
			return 0;
	}
	else
	{
		// exists and it's not a directory
		if (!(S_ISDIR(s.st_mode)))
			return 1;
	}
	// it's a directory, WHAT
	return 1;
}

/** Creates a file.
 *	@return 1 if successful, 0 if failed
 */
int create_file(char* path)
{
	FILE* file = fopen(path, "wb");
	if (!file)
		return 0;

	fclose(file);
	return 1;
}

/** Tells if the high score file is accessible.
 *	@return 0 if success, 1 if failed.
 */
int hscore_file_check()
{
	if (directory_exists(SCORE_DIR) == 1 && file_exists(SCORE_PATH) == 1)
		return 0;

	/* Try to create the directory */
	int retval = create_directory(SCORE_DIR);
	if (retval == -1)
		return 1;

	/* Try to create the file */
	retval = create_file(SCORE_PATH);
	if (retval == 0)
		return 1;

	retval = hscore_clean();
	if (retval == -1)
	{
		game.cant_open_hscore_file = 1;
		return 0;
	}

	/* And now, after creating them both... */
	if (directory_exists(SCORE_DIR) == 1 && file_exists(SCORE_PATH) == 1)
		return 0;

	return 1;
}

/** Restore the Highscores to the default.
 *
 *  @note If the Highscores file doesn't exist, it is created.
 */
int hscore_clean ()
{
	hscore_globals_init();

	/* Set highscores to default only if it's not already
	 * set. This way, scores are not reset between turns
	 * when the highscore file isn't available. */
	if (HIGH_SCORE_BORDERS == 0)
		HIGH_SCORE_BORDERS     = HIGH_SCORE_DEFAULT;

	if (HIGH_SCORE_BORDERS_OFF == 0)
		HIGH_SCORE_BORDERS_OFF = HIGH_SCORE_DEFAULT;

	FILE* file = fopen(SCORE_PATH, "wb");
	if (!file)
		return -1;

	fwrite (&HIGH_SCORE_BORDERS,     sizeof (int), 1, file);
	fwrite (&HIGH_SCORE_BORDERS_OFF, sizeof (int), 1, file);
	fclose (file);
    return 0;
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
	hscore_globals_init();

	int retval = hscore_file_check();
	if (retval == 1)
	{
		game.cant_open_hscore_file = 1;
		return;
	}

	FILE* file = fopen (SCORE_PATH, "rb");
	if (!file)
	{
		retval = hscore_clean();
		if (retval == -1)
		{
			game.cant_open_hscore_file = 1;
			return;
		}

		file = fopen (SCORE_PATH, "rb");
		if (!file) /* This time it really couldnt open the score file */
		{
			game.cant_open_hscore_file = 1;
			return;
		}
	}

	/* All right, opened the file! */
	int items_read = fread(&HIGH_SCORE_BORDERS, sizeof (int), 1, file);
	if (items_read != 1)
		nsnake_abort("Highscore File I/O error!\n"
					 "Try cleaning the scores file!");

	items_read = fread(&HIGH_SCORE_BORDERS_OFF, sizeof (int), 1, file);
	if (items_read != 1)
		nsnake_abort("Highscore File I/O error!\n"
					 "Try cleaning the scores file!");

	fclose(file);
}


/**	Records the player High Score
 */
void hscore_store ()
{
	game.cant_open_hscore_file = 0;

	int retval = hscore_file_check();
	if (retval == 1)
	{
		retval = hscore_clean();
		if (retval == -1)
		{
			game.cant_open_hscore_file = 1;
			return;
		}
	}

	FILE* file = fopen (SCORE_PATH, "r+b");
	if (!file)// We really couldn't open the high scores file
	{
		game.cant_open_hscore_file = 1;
		return;
	}

	if (game.mode == BORDERS_ON)
	{
		HIGH_SCORE_BORDERS = snake.score;
		fwrite (&HIGH_SCORE_BORDERS, sizeof (int), 1, file);
	}

	else if (game.mode == BORDERS_OFF)
	{
		HIGH_SCORE_BORDERS_OFF = snake.score;
		fseek (file, sizeof (int), SEEK_SET);
		fwrite (&HIGH_SCORE_BORDERS_OFF, sizeof (int), 1, file);
	}
	else
	{
		// oh my god, what should i do?
	}

	fclose (file);
}

