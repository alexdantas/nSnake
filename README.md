# nsnake

Classic snake game on the terminal

![menu](http://nsnake.alexdantas.net/images/arcade-menu.png)
![menu](http://nsnake.alexdantas.net/images/small-maze.png)
![menu](http://nsnake.alexdantas.net/images/large-maze-with-lots-of-fruits.png)

`nsnake` is a clone of the classic snake game that we all used to
play on our cellphones. You play this game on the terminal,
with textual interface.

Here's some features:

* Customizable gameplay, appearance and keybindings;
* Neat GUI-like interface with nice animations;
* Lots of possible game modes, with scores saved for each;

## Controls

The keybindings are completely customizable on the in-game menus.
The default ones are:

	Arrow Keys          Moves the snake
	q                   Quits the game at any time
	p                   Pauses/Unpauses the game
	h                   Show help during game

## Dependencies

Make sure you have the *ncurses dev* installed. Note that it's
_not_ the default that comes with your distro.

To get it, use you package manager - here's an example:

    # apt-get install libncurses5-dev

## Usage

Briefly, the following shell commands should configure,
build and install this package on the default directories:

    $ make
    $ [sudo] make install

If you want to test it before installing, do:

    $ make run

Then, it's simple:

	$ man nsnake
	$ nsnake

To change the default installation directories, check file
`INSTALL.md`.

## Contact

Hello there, I'm Alexandre Dantas (kure)!

Thanks for downloading and playing this game.
I'm glad you had interest on this project.
You can send me comments, bugs, ideas or anything else by email.
And if you have time, please visit my blog!

My email:           <eu@alexdantas.net>
My homepage:        http://www.alexdantas.net/
nsnake main page:   http://nsnake.alexdantas.net/

I'd appreciate any commentary - even if it's
just _"Hello, I play your game!"_.

## Credits

Firstly, I'd like to thank you for playing this game.
Hope you liked it!

The whole game design, coding, documenting, packaging and
testing were all made by me, on my free time.

But I didn't do everything! Throughout the nsnake development,
I found very interesting sources of ideas - such as games that also
uses nCurses for displaying graphics.
They have simple, short and commented source-codes that
can be used as references to learn C programming.

Special thanks to:

* *nInvaders*: A space invaders-like game using ncurses.
  * homepage: http://ninvaders.sourceforge.net
  * comments: "Thanks for the inspiration. If I haven't installed
               this in first place, I'd never have the idea
			   for nsnake"

* *pacman4console*: A console-based pacman game.
  * homepage:  http://doctormike.googlepages.com/pacman.html
  * comments:  "The way this package was organized
                    is incredible. Thanks for the
                    general packaging ideas"

* *vadorz*: An addicting ncurses space-invaders game.
  * homepage: http://code.google.com/p/vadorz/
  * comments: "This package is awesome, 'cause it uses ncurses
               AND pdcurses to be able to play in Windows and
			   GNU/Linux"

* *snake4*: Fruit-eating snake game
  * homepage: http://shh.thathost.com/
  * comments: "Thanks for the Makefile ideas. It's
               very well-written."

* *ASCII Generator*: A generator of awesome ASCII text arts
  * homepage:  http://www.network-science.de/ascii/

* *Text ASCII Art Generator*: Another awesome ASCII text generator
  * homepage:  http://patorjk.com/software/taag/
  * comments:  "I've used the Modular font to display
                the Game Over screen"


## License

 nsnake - The classic snake game with ncurses.
 Copyright (C) 2011,2014  Alexandre Dantas

 nsnake is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 homepage: http://www.alexdantas.net/projects/nsnake/
 mailto:   eu@alexdantas.net

