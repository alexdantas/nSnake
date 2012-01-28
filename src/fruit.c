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

/**	@file fruit.c
 *
 * 	Definition of the fruit functions
 */

#include <stdlib.h>
#include <time.h>

#include "fruit.h"
#include "engine.h"
#include "player.h"


struct fruit_t	fruit;


/**	Creates a fruit in a random place
 *
 * 	@note It doesn't verify if there is another fruit on the game
 */
void fruit_init ()
{
	int top = 50;
	int bottom = 10;
	int valid_fruit = FALSE;


	// Create a seed for generating random numbers based on the time
	srand (time (NULL));

	while (valid_fruit == FALSE)
	{
		// Generate a random number between the screen limits
		fruit.x = rand() % ((screen.width-2) - 1 + 1) + 1;
		fruit.y = rand() % ((screen.height-2) - 2 + 2) + 2;

		// Generates a random value between 'top' and 'bottom'
		fruit.bonus += rand() % (top - bottom + 1) + bottom;

		if (fruit.y != screen.height-1)
			valid_fruit = TRUE;

		// Lets make sure the fruit doesnt start at the snake body
		int i;
		for (i = 0; i < (snake.size-1); i++)
		{
			if ((fruit.x == snake.body[i].x) &&
			    (fruit.y == snake.body[i].y))
			{
				valid_fruit = FALSE;
				break;
			}
		}
	}
}


/**	Drops the bonus value of the fruit for each 'frame'
 */
void fruit_update_bonus ()
{
	if (fruit.bonus > 0)
		fruit.bonus--;
}
