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

/**	@file engine.h
 *	Declaration of the graphical engine functions, defines and globals
 */


#ifndef ENGINE_DEFINED
#define ENGINE_DEFINED


/** Global definitions related to the game screen
 */
struct screen_t
{
	/** The fixed width of the game area */
	int width;
	/** The fixed height of the game area */
	int height;
	/** The time (in microsseconds) that the game */
	int delay;
};


/** The global screen structure */
extern struct screen_t screen;


void draw_background ();
void draw_borders ();
void draw_fruit ();
void draw_fruit_bonus ();
void draw_player ();
void draw_score ();

void engine_exit ();
void engine_init ();
void engine_show_game_over ();
void engine_show_main_menu ();
void engine_show_pause ();
void engine_show_screen ();

int  get_main_menu_input (int* current_option);
void engine_get_game_input ();
void get_game_over_input ();
void get_pause_input ();


#endif
