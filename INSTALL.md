# nsnake INSTALL

Installing information for the game nsnake.
This applies to GNU/Linux systems.
Microsoft Windows users have a precompiled executable.

# Installation

Briefly, the following shell commands should configure,
build, and install this package on the default directories:

    $ make
    $ make install

# Installation Info

By default, `make install` installs the package on the following
directories:

| Directory                    | What to store            |
| ---------------------------- | -------------------------|
| `/usr/games/`                | Executable files         |
| `/usr/local/share/man/man6/` | Man page                 |
| `~/.local/share/nsnake/`     | User config and scores   |

# Advanced Installation

You can specify a custom root directory for the installation
(other than `/`).

To do this, give `make install` the option `DESTDIR=CUSTOM_ROOT`,
where `CUSTOM_ROOT` must be an absolute directory name.
For example:

    $ make install DESTDIR=test/dir

This way, the file hierarchy will be:

| Directory                            | What to store            |
| ------------------------------------ | -------------------------|
| `test/dir/usr/games/`                | Executable files         |
| `test/dir/usr/local/share/man/man6/` | Man page                 |
| `~/.local/share/nsnake/`             | User config and scores   |

You can also specify an installation prefix other than `/usr/local`
by giving `make` the option `PREFIX=CUSTOM_PREFIX`, where
`CUSTOM_PREFIX` is an absolute directory name.
For example:

    $ make install PREFIX=test/dir

This way, the file hierarchy will be:

| Directory                  | What to store            |
| -------------------------- | -------------------------|
| `test/dir/`                | Executable files         |
| `test/dir/share/man/man6/` | Man page                 |
| `~/.local/share/nsnake/`   | User config and scores   |

# Uninstallation

To uninstall this package:

    $ make uninstall

To purge (also remove the Highscore files, man page and
configurations) go for:

    $ make purge

# Advanced Uninstallation

If you specified a custom root directory for installation,
`make` needs to know about it to uninstall properly:

    $ make uninstall DESTDIR=test/dir

The same logic applies to purging:

    $ make purge DESTDIR=test/dir

If you installed it with a custom prefix, do the same:

    $ make uninstall PREFIX=test/dir

    $ make purge PREFIX=test/dir

    $ make purge SCOREDIR=test/dir

