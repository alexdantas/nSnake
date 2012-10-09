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

/**	@file   main.c
 *
 *	The 'main' function only
 */


#include "engine.h"
#include "fruit.h"
#include "nsnake.h"
#include "player.h"
#include "arguments.h"
#include "hscores.h"


/**	The main function - contains the main loop of the game.
 *
 *
 *	@note I tried to make this function as clear as possible, so anyone
 *        could understaing the whole game logic starting by here. @n
 *        Have fun with the source code!
 */
 int main (int argc, char* argv[])
{
	int c      = 0;
	int wait   = FALSE;

	if (argc > 1)
		args_handle (argc, argv);

	engine_init ();
	game.state = MAIN_MENU;

	while (TRUE == TRUE)
	{
		switch (game.state)
		{
		case MAIN_MENU:
			engine_show_main_menu ();
			game.state = GAME_INIT;
			break;

		case GAME_INIT:
			nsnake_init ();
			game.state = GAME;
			break;

		case GAME:
			if (snake.is_alive == FALSE)
			{
				game.state = GAME_OVER;
				break;
			}

			switch (c = engine_get_game_input ())
			{
			case ERR:
				// If we get no input
				break;

			case KEY_UP:    case 'w': case 'W':
				player_change_direction (UP);
				break;

			case KEY_LEFT:  case 'a': case 'A':
				player_change_direction (LEFT);
				break;

			case KEY_DOWN:  case 's': case 'S':
				player_change_direction (DOWN);
				break;

			case KEY_RIGHT: case 'd': case 'D':
				player_change_direction (RIGHT);
				break;

			case 'q':	case 'Q':
				game.state = EXIT;
				break;

		#ifdef DEBUG //debug key to increase score and size (gcc -DDEBUG)
			case 'e':	case 'E':
				player_increase_score (100);
				player_increase_size (2);
				break;
		#endif
			case 'p':	case 'P':
				game.state = PAUSED;
				break;

			default:
				break;
			}

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
			break;

		case PAUSED:
			engine_show_pause ();

			wait = TRUE;
			while (wait == TRUE)
			{
				int input = engine_get_game_input ();

				switch (input)
				{
				case 'p':	case 'P':
					wait = FALSE;
					game.state = GAME;
					break;

				case 'q':	case 'Q':
					wait = FALSE;
					game.state = EXIT;
					break;

				default:
					break;
				}

				engine_clean_pause ();
			}
			break;

		case GAME_OVER:
			if (game.mode == BORDERS_ON)
			{
				if (snake.score > HIGH_SCORE_BORDERS)
					hscore_store ();
			}
			else if (game.mode == BORDERS_OFF)
			{
				if (snake.score > HIGH_SCORE_BORDERS_OFF)
					hscore_store ();
			}

			engine_show_game_over ();

			wait = TRUE;
			while (wait == TRUE)
			{
				switch (c = engine_get_game_input())
				{
				case 'q':	case 'Q':
					wait = FALSE;
					game.state = EXIT;
					break;

				case 'm':	case 'M':
					wait = FALSE;
					game.state = MAIN_MENU;
					break;

#if OS_IS_WINDOWS
				case ' ':
#else
				case '\n':
#endif
					wait = FALSE;
					game.state = GAME_INIT;
					break;

				default:
					break;
				}
			}

			engine_clean_game_over();
			break;

		case EXIT:
			engine_exit ();
			nsnake_exit ();
			break;

		default:
			break;
		}
	}

	// Even though we have this here, the game always quits during
	// the main loop
	engine_exit ();
	nsnake_exit ();
	return 0;
}
