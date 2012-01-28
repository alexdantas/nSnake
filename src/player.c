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

/**	@file player.c
 *
 *	Definition of the player functions
 */

#include <stdlib.h>

#include "nsnake.h"
#include "engine.h"
#include "player.h"
#include "fruit.h"


struct player_t	snake;


/**	Changes the snake direction based on the input received
 */
void player_change_direction (int new_direction)
{
	if      ((new_direction == UP) && (snake.direction != DOWN) &&
	                                  (snake.direction != UP)        )
		snake.direction = UP;


	else if ((new_direction == LEFT) && (snake.direction != RIGHT) &&
	                                    (snake.direction != LEFT)    )
		snake.direction = LEFT;

	else if ((new_direction == DOWN) && (snake.direction != UP) &&
	                                    (snake.direction != DOWN)    )
		snake.direction = DOWN;

	else if ((new_direction == RIGHT) && (snake.direction != LEFT) &&
                                             (snake.direction != RIGHT)   )
		snake.direction = RIGHT;
}


/**	Depending on the game mode, the player may be able to hit borders
 *  or not. If it is possible, we call player_hit_borders().
 *
 *  @see player_hit_borders()
 *  @return FALSE, if the player didn't hit the borders, and
 *          TRUE if it did.
 */
int player_collided_with_borders ()
{
	if (game.mode == BORDERS_ON)
		return player_hit_borders ();

	else if (game.mode == BORDERS_OFF)
		player_teleport_borders ();

	else if (game.mode == FREE_MODE)	//secret debugging-mode
	{}

	return FALSE;
}


/** Free all memory associated with the player.
 */
void player_exit ()
{
	if (snake.body != NULL)
	{
		free (snake.body);
		snake.body = NULL;
	}
}


/**	Checks collision between the snake and the walls.
 *
 * 	@warning	Behind you!
 */
int player_hit_borders ()
{
	if ((snake.body[0].x < 1) || (snake.body[0].x > (screen.width-2)) ||
	    (snake.body[0].y < 2) || (snake.body[0].y > (screen.height-2)))
	{
		return TRUE;
	}
	return FALSE;
}


/**	Checks if the snake has eaten the fruit.
 *
 *  @return 1 if yes, 0 if no.
 */
int player_hit_fruit ()
{
	if ((snake.body[0].x == fruit.x) && (snake.body[0].y == fruit.y))
		return TRUE;
	else
		return FALSE;
}


/**	Checks if the snake have collided with itself
 */
int player_hit_self ()
{
	int i;
	for (i = (snake.size-1); i > 1; i--)
	{
		if ((snake.body[0].x == snake.body[i].x) &&
		    (snake.body[0].y == snake.body[i].y))
		{
			return TRUE;
		}
	}
	return FALSE;
}


/**	Simply increases the score by the value sent as the parameter.
 */
void player_increase_score (int add)
{
	snake.score += add;
}


/**	Increases the snake size.
 *
 * 	Here we have the core function of this file. Each time the snake
 * 	increases its size, we expand the array that represents its body.
 *
 * 	@note This is the main function of this file, because it shows
 *        how i've implemented the snake. In the future, we could
 *        use a linked list instead of an array.
 */
void player_increase_size (int size)
{
	int piece_size = sizeof (struct player_pieces);

	snake.size += size;

	snake.body = realloc (snake.body, (snake.size * piece_size));
	if (snake.body == NULL)
		nsnake_abort ("Memory Error!\n");
}


/**	Starts the player-related variables.
 */
void player_init ()
{
	snake.body = NULL;

	snake.is_alive  = TRUE;
	snake.speed	    = 1;
	snake.score	    = 0;
	snake.direction = RIGHT;

	snake.size = 3;
	snake.body = malloc (snake.size * sizeof (struct player_pieces));
	if (snake.body == NULL)
		nsnake_abort ("Memory Error!\n");

	int i;
	for (i = 0; i < snake.size; i++)
	{
		snake.body[i].x = screen.width/2;
		snake.body[i].y = screen.height/2;
	}
}


/**	Teleports the player around the borders.
 */
void player_teleport_borders ()
{
	if (snake.body[0].x < 1)
		snake.body[0].x = screen.width-2;

	if (snake.body[0].y < 2)
		snake.body[0].y = screen.height-2;

	if (snake.body[0].x > (screen.width-2))
		snake.body[0].x = 1;

	if (snake.body[0].y > (screen.height-2))
		snake.body[0].y = 2;
}


/**	Updates the player position, one piece at a time
 *
 *	Start by moving the snake pieces one at a time from the last to the
 *	first, and then moves the head according to its direction.
 */
void player_update ()
{
// body
	int i;
	for (i = (snake.size-1); i > 0; i--)
	{
		snake.body[i].x = snake.body[i-1].x;
		snake.body[i].y = snake.body[i-1].y;
	}

// head
	if      (snake.direction == UP)
		snake.body[0].y -= snake.speed;

	else if (snake.direction == LEFT)
		snake.body[0].x -= snake.speed;

	else if (snake.direction == DOWN)
		snake.body[0].y += snake.speed;

	else if (snake.direction == RIGHT)
		snake.body[0].x += snake.speed;
}
