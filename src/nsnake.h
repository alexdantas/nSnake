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

/** @file nsnake.h
 *
 *	This file defines the core functions, variables and structures.
 */

#ifndef NSNAKE_DEFINED
#define NSNAKE_DEFINED


#ifndef TRUE
  /** Boolean type TRUE - this will make things easier to read */
  #define TRUE  1
#endif

#ifndef FALSE
  /** Boolean type FALSE - this will make things easier to read */
  #define FALSE 0
#endif

typedef enum { MAIN_MENU, GAME_INIT, GAME, PAUSED, GAME_OVER, EXIT } states;

/** The possible Game Modes.
 *
 *  In Normal Mode, the snake dies by colliding with itself or the borders. @n
 *  In Teleport Mode, the snake only dies by colliding with itself. @n
 *  @note	Free Mode is for testing purposes only.
 */
typedef enum { FREE_MODE, BORDERS_ON, BORDERS_OFF } modes;

/** Global definitions related to the game in general.
 */
struct game_t
{
	modes  mode; /**< The collision mode of the game */
	int    level; /**< How fast will the snake move */
	states state; /**< The current game state */
	int    cant_open_hscore_file; /**< Flag that tells if we could open the highscores file */
};

/** The global game structure */
extern struct game_t game;



void nsnake_abort (char* error_msg);
void nsnake_exit ();
void nsnake_game_over ();
void nsnake_init ();
void nsnake_pause ();


#endif
