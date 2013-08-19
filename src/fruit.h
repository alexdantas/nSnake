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

/**	@file fruit.h
 *
 *  Declaration of the fruit functions, defines and globals
 */

#ifndef FRUIT_DEFINED
#define FRUIT_DEFINED


/**	The fruit structure.
 *
 * 	Each fruit has it x and y positions
 * 	and a bonus score - indicating how many extra points
 * 	will be rewarded to the snake when it eats the fruit.
 */
struct fruit_t
{
	int x;     /**< x position of the fruit on stage */
	int y;     /**< y position of the fruit on stage */
	int bonus; /**< Bonus that the fruit adds to the score when eaten */

};


/** One global copy of the fruit */
extern struct fruit_t fruit;


void fruit_init ();
void fruit_update_bonus ();


#endif
