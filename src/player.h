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

/**	@file   player.h
 *	Declaration of the player functions, defines and globals
 */

#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED


#include "nsnake.h"


/**	The structure of each segmentation of the player.
 */
struct player_pieces_t
{
	/** The x position of this piece */
	int x;
	/** The y position of this piece */
	int y;
};


/** A simple enum to make the directions of the player easier to read.
 */
enum directions { UP = 0, LEFT, DOWN, RIGHT };


/**	Represent the snake itself.
 */
struct player_t
{
	/** Indicates if the player is alive */
	int is_alive;

	/** How many chars will the player move by frame */
	int	speed;

	/** The current size of the snake.pieces's body */
	int	size;

	/** The score :D */
	int score;

	/** Which direction shoud the player go */
	int direction;

	/** All pieces of the snake (including the head) */
	struct player_pieces_t* body;
};


/** The player himself */
extern struct player_t snake;


void player_change_direction (int new_direction);
int player_collided_with_borders ();
void player_exit ();
int  player_hit_borders ();
int  player_hit_fruit ();
int  player_hit_self ();
void player_increase_score (int add);
void player_increase_size (int size);
void player_init ();
void player_teleport_borders ();
void player_update ();


#endif
