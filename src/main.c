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

/**	@file   main.c
 *	The 'main' function only
 */


#include "engine.h"
#include "fruit.h"
#include "nsnake.h"
#include "player.h"
#include "arguments.h"


/**	The main function - contains the main loop of the game.
 *
 *
 *	@note I tried to make this function as clear as possible, so anyone
 *        could understaing the whole game logic starting by here. @n
 *        Have fun with the source code!
 */
 int main (int argc, char* argv[])
{
	if (argc > 1)
		args_handle (argc, argv);

	engine_init ();
	engine_show_main_menu ();
	nsnake_init ();

	while (TRUE == TRUE)
	{
		if (snake.is_alive == FALSE)
			nsnake_game_over ();

		engine_get_game_input ();

		player_update ();
		fruit_update_bonus ();

		if (player_hit_fruit () == TRUE)
		{
			// Is this score arbitrary?
			player_increase_score (game.level*3 + fruit.bonus);
			player_increase_size (2);
			fruit_init ();
		}

		if (player_hit_self () == TRUE)
			snake.is_alive = FALSE;

		if (player_collided_with_borders () == TRUE)
			snake.is_alive = FALSE;

		engine_show_screen ();
	}


	// Even though we have this here, the game always quits during
	// the main loop
	engine_exit ();
	nsnake_exit ();
	return 0;
}
