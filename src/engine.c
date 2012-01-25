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

/** @file engine.c
 *	Definition of the graphical engine functions
 *
 *	This file presents the nCurses specifical implementation of the game`s
 * 	graphics.
 */


#include <stdio.h>
#include <stdlib.h>

#include <ncurses.h> // the actual game engine

#include <unistd.h>  // for the usleep () function

#include "engine.h"
#include "fruit.h"
#include "player.h"
#include "hscores.h"


/** This formula determines a time (in microseconds) the game must wait
 *  between each screen refresh. It is based on the current game level,
 *  so the higher its value, the lowest is the delay and faster the game
 *  will appear to be.
 *
 *  @note 1,000,000 microseconds == 1 second
 */
#define   REFRESH_DELAY     (20000 + ((9 - game.level) * 25000))



static char PLAYER_HEAD_CHAR = '@'; ///< The 'image' of the snake head
static char PLAYER_CHAR	     = 'o'; ///< The 'image' of the snake body
static char FRUIT_CHAR	     = '$'; ///< The 'image' of the fruit
static char BORDER_CHAR	     = '#'; ///< The 'image' of the border in normal mode
static char NO_BORDER_CHAR 	 = '.'; ///< The 'image' of the border in teleport mode
static char MENU_BORDER_CHAR = '*'; ///< The 'image' of the border in the main menu



/** Simple enum to make the colors easier to read: FOREGROUND_BACKGROUND */
enum Colors { BLACK_WHITE = 1, CYAN_BLACK, BLUE_BLACK,
              WHITE_BLACK, GREEN_BLACK, RED_BLACK,  };



struct screen_t screen;



/**	Just erases everything to black
 */
void draw_background ()
{
	clear();
}


/**	Draws the window border, according to the game.mode.
 */
void draw_borders ()
{
	int i;

	attron (COLOR_PAIR (WHITE_BLACK));
	if (game.mode == BORDERS_ON)
	{
		for (i = 0; i <= (screen.width-1); i++)	//upper
		{
			mvaddch (1, i, BORDER_CHAR);
			mvaddch ((screen.height-1), i, BORDER_CHAR);
		}
		for (i = 1; i <= (screen.height-1); i++)	//lower
		{
			mvaddch (i, 0, BORDER_CHAR);
			mvaddch (i, (screen.width-1), BORDER_CHAR);
		}
	}
	else if (game.mode == BORDERS_OFF)
	{
		for (i = 0; i <= (screen.width-1); i++)
		{
			mvaddch (1, i, NO_BORDER_CHAR);
			mvaddch ((screen.height-1), i, NO_BORDER_CHAR);
		}
		for (i = 1; i <= (screen.height-1); i++)
		{
			mvaddch (i, 0, NO_BORDER_CHAR);
			mvaddch (i, (screen.width-1), NO_BORDER_CHAR);
		}
	}
}


/**	Draws the current fruit on screen
 */
void draw_fruit ()
{
	attron (COLOR_PAIR (CYAN_BLACK));
	mvaddch (fruit.y, fruit.x, FRUIT_CHAR);
}


/**	Draws the bonus value of the current fruit
 */
void draw_fruit_bonus ()
{
	attron (COLOR_PAIR (WHITE_BLACK));
	mvprintw (0, (screen.width-1)/2, "Bonus: %d", fruit.bonus);
}


/**	Draws the snake - from the head to the whole body
*/
void draw_player ()
{
	attron (COLOR_PAIR (GREEN_BLACK));
	mvaddch (snake.body[0].y, snake.body[0].x, PLAYER_HEAD_CHAR);

	int i;
	for (i = 1; i < snake.size; i++)
		mvaddch (snake.body[i].y, snake.body[i].x, PLAYER_CHAR);
}


/**	Prints the current score
 */
void draw_score ()
{
	attron (COLOR_PAIR (CYAN_BLACK));
	mvprintw (0, 0, "nSnake v");
	mvprintw (0, 9, VERSION);

	attron (COLOR_PAIR (WHITE_BLACK));
	mvprintw (0, 17, "Lv%d", game.level);
	mvprintw (0, 23, "Score: %d", snake.score);

	if (game.mode == BORDERS_ON)
		mvprintw (0, 60, "High Score: %d", HIGH_SCORE_BORDERS);
	else if (game.mode == BORDERS_OFF)
		mvprintw (0, 60, "High Score: %d", HIGH_SCORE_BORDERS_OFF);
}


/**	Exits and dealocates the memory required by ncurses.
 */
void engine_exit ()
{
	clear ();
	refresh ();
	// Effectively ends ncurses mode
	endwin ();
}


/**	Get the user input during game and make the right decisions
 */
void engine_get_game_input ()
{
	// The input variable MUST be int to accept non-ascii characters
	int input = getch ();

	switch (input)
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
		engine_exit ();
		nsnake_exit ();
		break;

#ifdef DEBUG //debug key to increase score and size (gcc -DDEBUG)
	case 'e':	case 'E':
		player_increase_score (100);
		player_increase_size (2);
		break;
#endif
	case 'p':	case 'P':
		engine_show_pause ();
		break;

	default:
		break;
	}
}


/**	Starts the game engine. Initializes all the stuff related to ncurses.
 *
 *  @note If some engine-specific initialization fails, the game aborts.
 */
void engine_init ()
{
	screen.width  = 80;
	screen.height = 24;

	// Starts the ncurses mode
	initscr ();

	if (has_colors() == FALSE)
		nsnake_abort ("Your terminal does not support colors.\n");

	// Start support for colors ( Name, Foreground, Background )
	start_color ();
	init_pair (GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
	init_pair (CYAN_BLACK,  COLOR_CYAN,  COLOR_BLACK);
	init_pair (WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
	init_pair (RED_BLACK,   COLOR_RED,   COLOR_BLACK);
	init_pair (BLUE_BLACK,  COLOR_BLUE,  COLOR_BLACK);
	init_pair (BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);

	int current_height, current_width;
	// Gets the current width and height of the terminal
	getmaxyx (stdscr, current_height, current_width);

	if ((current_width < screen.width) || (current_height < screen.height))
		nsnake_abort ("Your console screen is smaller than 80x24\n"
		               "Please resize your window and try again\n\n");

	// Character input doesnt require the <enter> key anymore
	raw ();

	// Makes the cursor invisible
	curs_set (0);

	// Support for extra keys (life F1, F2, ... )
	keypad (stdscr, true);

	// Wont print the input received
	noecho ();

	// Wont wait for input - the game will run instantaneously
	nodelay (stdscr, true);

	// Refresh the screen (prints whats in the buffer)
	refresh ();

	game.mode = BORDERS_ON;
}


/**	Draws the Game Over screen.
 *
 *	Besides drawing 'Game Over', it highlights where the player died.
 */
void engine_show_game_over ()
{
	attron (COLOR_PAIR (RED_BLACK));
	mvaddch (snake.body[0].y, snake.body[0].x, 'x');

	mvprintw (3, 22,  " _______  _______  __   __  _______ ");
	mvprintw (4, 22,  "|       ||   _   ||  |_|  ||       |");
	mvprintw (5, 22,  "|    ___||  |_|  ||       ||    ___|");
	mvprintw (6, 22,  "|   | __ |       ||       ||   |___ ");
	mvprintw (7, 22,  "|   ||  ||       ||       ||    ___|");
	mvprintw (8, 22,  "|   |_| ||   _   || ||_|| ||   |___ ");
	mvprintw (9, 22,  "|_______||__| |__||_|   |_||_______|");
	mvprintw (10, 22, " _______  __   __  _______  ______  ");
	mvprintw (11, 22, "|       ||  | |  ||       ||    _ | ");
	mvprintw (12, 22, "|   _   ||  |_|  ||    ___||   | || ");
	mvprintw (13, 22, "|  | |  ||       ||   |___ |   |_|| ");
	mvprintw (14, 22, "|  |_|  ||       ||    ___||    __ |");
	mvprintw (15, 22, "|       | |     | |   |___ |   |  ||");
	mvprintw (16, 22, "|_______|  |___|  |_______||___|  ||");
	mvprintw (17, 28,       "Press <enter> to retry");
	mvprintw (18, 31,          "<m> to Main Menu");
	draw_score ();

	refresh ();
}


/**	Displays the main menu and gets the user input from it.
 *
 *  This function blocks the game execution and waits for user input,
 *  refreshing the main menu screen according to the options selected.
 */
void engine_show_main_menu ()
{
	int wait = TRUE;

	int speed_option = 1;
	char speed_options[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

	int menu_x_padding = 13;
	int option_x_padding = menu_x_padding + 17;

	clear ();

	while (wait == TRUE)
	{
		// The borders
		attron (COLOR_PAIR (WHITE_BLACK));
		int i;
		for (i = 0; i < screen.width; i++)
		{
			mvaddch (0, i, MENU_BORDER_CHAR);
			mvaddch (screen.height - 1, i, MENU_BORDER_CHAR);
		}
		for (i = 0; i < screen.height; i++)
		{
			mvaddch (i, 0, MENU_BORDER_CHAR);
			mvaddch (i, screen.width - 1, MENU_BORDER_CHAR);
		}

		attron (COLOR_PAIR (GREEN_BLACK));
		mvprintw(1, 3,  "         ,d8888b.                     888");
		mvprintw(2, 3,  "        d88P  Y88b                    888");
		mvprintw(3, 3,  "         Y88b.                        888");
		mvprintw(4, 3,  "88888b.   Y888b.    88888b.    8888b. 888  888  .d88b.");
		mvprintw(5, 3,  "888  88b     8Y88b. 888  88b      88b 888 .88P d8P  Y8b");
		mvprintw(6, 3,  "888  888       888b 888  888 .d888888 888888K  88888888");
		mvprintw(7, 3,  "888  888 Y88b  d88P 888  888 888  888 888  88b Y8b.");
		mvprintw(8, 3,  "888  888   Y8888P   888  888  Y888888 888  888  Y88888");

		attron (COLOR_PAIR (CYAN_BLACK));
		mvaddch (8, 59, 'v');
		mvprintw (8, 60, VERSION);

		attron (COLOR_PAIR (BLUE_BLACK));
		mvprintw (10, 5, " ___________________________________________________ ");
		mvprintw (11, 5, "|                                                   |");
		mvprintw (12, 5, "|                                                   |");
		mvprintw (13, 5, "|                                                   |");
		mvprintw (14, 5, "|                                                   |");
		mvprintw (15, 5, "|                                                   |");
		mvprintw (16, 5, "|                                                   |");
		mvprintw (17, 5, "|                                                   |");
		mvprintw (18, 5, "|                                                   |");
		mvprintw (19, 5, "|___________________________________________________|");
		mvprintw (12, menu_x_padding, "Press <enter> or <space> to start game");
		mvprintw (13, menu_x_padding, "Press <q> to quit game");

		// Here we draw the game mode
		mvprintw (15, menu_x_padding, "Game Mode:");
		if (game.mode == BORDERS_ON)
		{
			attron (COLOR_PAIR (WHITE_BLACK));
			mvprintw (15, option_x_padding, "Borders On");

			attron (COLOR_PAIR (BLUE_BLACK));
			mvprintw (16, option_x_padding, "Borders Off");
		}
		else
		{
			attron (COLOR_PAIR (BLUE_BLACK));
			mvprintw (15, option_x_padding, "Borders On");

			attron (COLOR_PAIR (WHITE_BLACK));
			mvprintw (16, option_x_padding, "Borders Off");
		}

		// And here we draw the level numbers
		attron (COLOR_PAIR (BLUE_BLACK));
		mvprintw (18, menu_x_padding, "Starting speed:");

		// Tricky, draw the options with the right colors
		int j;
		for (i = 0, j = 0; i < 9; i++)
		{
			if (i == (speed_option-1))
				attron (COLOR_PAIR (WHITE_BLACK));
			else
				attron (COLOR_PAIR (BLUE_BLACK));

			mvprintw (18, option_x_padding+j, "%c", speed_options [i]);
			j += 2;
		}

		attron (COLOR_PAIR (WHITE_BLACK));
		mvprintw (screen.height-2, 2, "Use --help for guidelines");

		// Now we wait for orders
		wait = get_main_menu_input (&speed_option);

		// This function is so refreshing...
		refresh ();
	}

	game.level = speed_option;
}


/**	Prints the pause message
 */
void engine_show_pause ()
{
	attron (COLOR_PAIR (RED_BLACK));

	mvprintw ((screen.height-1)/2, ((screen.width-1)/2)-5, "Game Paused ");
	mvprintw (((screen.height-1)/2)+1, ((screen.width-1)/2)-11, "Press <p> to Continue...");

	get_pause_input ();
}


/**	Completely draws the screen during game.
 *
 * 	The usleep() function interrupts the program for 'n' microseconds.
 * 	It was difficult to get a stable value for the game progression.
 *
 *	@note This is the main function of this file because it shows
 * 	      logically how the process of drawing the screen sould be
 */
void engine_show_screen ()
{
	draw_background ();
	draw_borders ();
	draw_fruit ();
	draw_player ();
	draw_fruit_bonus();
	draw_score ();

	usleep (REFRESH_DELAY);

	refresh();
}


/**	Waits for an user action during the 'Game Over' screen
 */
void get_game_over_input ()
{
	int wait = TRUE;

	while (wait == TRUE)
	{
		int input = getch();

		switch (input)
		{
		case ERR:
			// if we get no input
			break;

		case 'q':	case 'Q':
			engine_exit ();
			nsnake_exit ();
			break;

		case 'm':	case 'M':
			wait = FALSE;
			engine_show_main_menu ();

		case '\n':
			wait = FALSE;
			break;

		default:
			break;
		}
	}
}


/**	Gets the input for the main menu.
 */
int get_main_menu_input (int* speed_option)
{
	int input = getch();

	switch (input)
	{
	case ERR:
		// if we get no input
		break;

	case '\n':	case ' ':
		return FALSE;
		break;

	case 'q':	case 'Q':
		engine_exit();
		nsnake_exit();
		break;

	case KEY_UP:
		if (game.mode == BORDERS_OFF)
			game.mode = BORDERS_ON;
		break;

	case KEY_DOWN:
		if (game.mode == BORDERS_ON)
			game.mode = BORDERS_OFF;
		break;

	case KEY_LEFT:
		if (*speed_option > 1)
			(*speed_option)--;
		break;

	case KEY_RIGHT:
		if (*speed_option < 9)
			(*speed_option)++;
		break;

	case '1':
		*speed_option = 1;
		break;
	case '2':
		*speed_option = 2;
		break;
	case '3':
		*speed_option = 3;
		break;
	case '4':
		*speed_option = 4;
		break;
	case '5':
		*speed_option = 5;
		break;
	case '6':
		*speed_option = 6;
		break;
	case '7':
		*speed_option = 7;
		break;
	case '8':
		*speed_option = 8;
		break;
	case '9':
		*speed_option = 9;
		break;

	default:
		break;
	}

	return TRUE;
}


/**	Just waits untill the user either unpauses the game or exits
 */
void get_pause_input ()
{
	int paused = TRUE;

	// Makes the cursor visible
	curs_set (1);

	while (paused == TRUE)
	{
		int input = getch ();

		switch (input)
		{
		// If we get no input
		case ERR:
			break;

		case 'p':	case 'P':
			paused = FALSE;
			break;

		case 'q':	case 'Q':
			engine_exit ();
			nsnake_exit ();
			break;

		default:
			break;
		}
	}

	// And here it becomes invisible again
	curs_set (0);
}
