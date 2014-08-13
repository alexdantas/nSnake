# nSnake Installation How-To

This file tells you how to manually install _nsnake_ on GNU/Linux systems.

First, I recommend you to check
[if _nsnake_ is already available on your distro][distro]. In this case,
you most likely won't need to install manually.

## Dependencies

nSnake only depends on `ncurses`; it's used to show cute things on the terminal.

Make sure you have the package *ncurses dev*. Note that it's _not_ the default
that comes with your distro.

| Distro         | Installation command              |
| -------------- | --------------------------------- |
| Ubuntu/Debian  | `apt-get install libncurses5-dev` |
| Fedora         | `yum install ncurses-devel`       |
| Arch Linux     | _comes by default_                |

If you know the means to install on other distros, [please tell me][issues].

## Installation

Briefly, the following shell commands should configure,
build, and install this package on the default directories:

    $ make
    $ make install

If you want to test _nsnake_ before installing, do `make run`.

By default, `make install` puts the game on the following
directories:

| Directory                        | What to store            |
| -------------------------------- | -------------------------|
| `/usr/bin/`                      | Executable file          |
| `/usr/share/man/man6/`           | Man page                 |
| `/usr/share/icons/hicolor`       | PNG icons                |
| `/usr/share/pixmaps`             | XPM icon                 |
| `/usr/share/applications`        | Desktop file             |
| `/usr/share/games/nsnake/levels` | Default game levels      |
| `~/.local/share/nsnake/`         | User config and scores   |

## Advanced Installation

You can customize the install locations by using some `make`
variables.

`$DESTDIR` allows you to specify a custom root directory for the
whole game (other than `/`).

To do this, give `make install` the option `DESTDIR=CUSTOM_ROOT`;
for example:

    $ make install DESTDIR=test/dir

This way, all files will be put inside `test/dir`, like this:

| Directory                            | What to store            |
| ------------------------------------ | -------------------------|
| `test/dir/usr/bin/`                  | Executable file          |
| `test/dir/usr/share/man/man6/`       | Man page                 |
| `test/dir/usr/share/icons/hicolor`   | PNG icons                |
| ...                                  | ...

`$PREFIX` allows you to specify an installation prefix other than
`/usr` by giving `make` the option `PREFIX=CUSTOM_PREFIX`;
for example:

    $ make install PREFIX=other/dir

This way, the file hierarchy will be:

| Directory                       | What to store            |
| ------------------------------- | -------------------------|
| `other/dir/bin`                 | Executable files         |
| `other/dir/share/man/man6/`     | Man page                 |
| `other/dir/share/icons/hicolor` | PNG icons                |
| ...                             | ...                      |

## Uninstallation

To uninstall this package and all it's files:

    $ make uninstall

Note that if you specified a custom root directory for installation,
`make` needs to know about it to uninstall properly:

    $ make uninstall DESTDIR=test/dir

If you installed it with a custom prefix, do the same:

    $ make uninstall PREFIX=other/dir

[distro]: http://nsnake.alexdantas.net/#installation
[issues]: https://github.com/alexdantas/nSnake/issues

