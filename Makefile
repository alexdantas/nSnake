# nsnake Makefile
# (2013-2014) Alexandre Dantas <eu@alexdantas.net>
#
# This is a rather complex Makefile, sorry about that.
# It supports the following targets:
#
# make all        Builds the package
# make run        Builds and runs the program
# make install    Installs the package on your system
# make uninstall  Uninstalls the package from your system
# make clean      Cleans results of building process
# make clean-all  Cleans everything (project files and libs)
# make dist       Creates source code "tarball"
# make doc        Generates the documentation with doxygen
# make docclean   Removes the documentation
#
# Also, the following commandline arguments customize
# default actions:
#
#  V        Verbose mode, off by default.
#           To turn on for the current command,
#           add `V=1` when calling `make`.
#           To turn on permanently, uncomment the line
#           specified below
#  DESTDIR  Installs the package on a custom root directory
#           (other than `/`). For example `DESTDIR=~/`.
#  PREFIX   Installs the package on a custom directory
#           (overwrites root)
#  CFLAGS   Changes the C flags used on compilation
#  CDEBUG   If you wish to build on debug mode, add 'CDEBUG=-g'
#  CFLAGS_PLATFORM
#           User specified compiler flags
#  LDFLAGS_PLATFORM
#           User specified linker flags
#

# Uncomment line below to tun on verbose mode permanently
#V = 1;

# General Info
PACKAGE = nsnake
VERSION = 3.0.1
DATE    = $(shell date "+%b%Y")

# Install dirs
PREFIX      = /usr
EXEC_PREFIX = $(PREFIX)
DATAROOTDIR = $(PREFIX)/share
BINDIR      = $(EXEC_PREFIX)/bin

# Misc stuff
PNGDIR     = $(DATAROOTDIR)/icons/hicolor
XPMDIR     = $(DATAROOTDIR)/pixmaps
DESKTOPDIR = $(DATAROOTDIR)/applications
LEVELDIR   = $(DATAROOTDIR)/games/nsnake/levels

# Things for the man page
MANROOT   = $(DATAROOTDIR)/man
MANDIR    = $(MANROOT)/man$(MANNUMBER)
MANNUMBER = 6
MANFILE   = $(PACKAGE).$(MANNUMBER)
MANPAGE   = doc/man/$(MANFILE)

# Build info
EXE         = $(PACKAGE)
CDEBUG      = -O2
CXXFLAGS    = $(CDEBUG) -Wall -Wextra $(CFLAGS_PLATFORM)
LDFLAGS     = -lncurses $(LDFLAGS_PLATFORM)
INCLUDESDIR = -I"src/" -I"deps/"
LIBSDIR     =

# Project source files
CFILES   = $(shell find src -type f -name '*.c')
CXXFILES = $(shell find src -type f -name '*.cpp')
OBJECTS  = $(CFILES:.c=.o) \
           $(CXXFILES:.cpp=.o)

# Engine source files
ENGINE_DIR     = deps/Engine
ENGINE_FILES   = $(shell find $(ENGINE_DIR) -type f -name '*.cpp')
ENGINE_OBJECTS = $(ENGINE_FILES:.cpp=.o)

# Commander source files
COMMANDER_DIR     = deps/commander
COMMANDER_FILES   = $(shell find $(COMMANDER_DIR) -type f -name '*.c')
COMMANDER_OBJECTS = $(COMMANDER_FILES:.c=.o)
COMMANDER_CFLAGS  = -O2 -Wall -Wextra $(CFLAGS_PLATFORM)

DEFINES = -DVERSION=\""$(VERSION)"\"                  \
          -DPACKAGE=\""$(PACKAGE)"\"                  \
          -DDATE=\""$(DATE)"\"                        \
          -DSYSTEM_LEVEL_DIR=\""$(LEVELDIR)"\"

# Distribution tarball
TARNAME = $(PACKAGE)
DISTDIR = $(TARNAME)-$(VERSION)

# Verbose mode check
ifdef V
MUTE =
VTAG = -v
else
MUTE = @
endif

ifdef DESTDIR
ROOT = -
else
ROOT =
endif

ifdef DEBUG
CDEBUG = -D_NSNAKE_DEBUG
else
CDEBUG =
endif

# Make targets
all: dirs $(EXE)
	# Build successful!

install: all
	# Installing...
	$(MUTE)install -pdm755 $(DESTDIR)$(BINDIR)
	$(MUTE)install -pm755 bin/$(EXE) $(DESTDIR)$(BINDIR)

	-$(MUTE)cat $(MANPAGE) | sed -e "s|DATE|$(DATE)|g" -e "s|VERSION|$(VERSION)|g" >$(MANFILE)
	$(MUTE)install -pdm755 $(DESTDIR)$(MANDIR)
	$(MUTE)install -pm644 $(MANFILE) $(DESTDIR)$(MANDIR)
	$(MUTE)rm -f $(MANFILE)

	$(MUTE)install -pdm755 $(DESTDIR)$(LEVELDIR)
	$(MUTE)install -pm644 levels/* $(DESTDIR)$(LEVELDIR)

	$(MUTE)install -pdm755 $(DESTDIR)$(PNGDIR)/16x16/apps/
	$(MUTE)install -pm644 misc/nsnake16.png $(DESTDIR)$(PNGDIR)/16x16/apps/nsnake.png
	$(MUTE)install -pdm755 $(DESTDIR)$(PNGDIR)/32x32/apps/
	$(MUTE)install -pm644 misc/nsnake32.png $(DESTDIR)$(PNGDIR)/32x32/apps/nsnake.png
	$(MUTE)install -pdm755 $(DESTDIR)$(XPMDIR)
	$(MUTE)install -pm644 misc/nsnake32.xpm $(DESTDIR)$(XPMDIR)/nsnake.xpm
	$(MUTE)install -pdm755 $(DESTDIR)$(DESKTOPDIR)
	$(MUTE)install -pm644 misc/nsnake.desktop $(DESTDIR)$(DESKTOPDIR)

	# $(PACKAGE) successfuly installed!

uninstall:
	# Uninstalling...
	$(MUTE)rm -f $(DESTDIR)$(BINDIR)/$(EXE)
	$(MUTE)rm -f $(DESTDIR)$(MANDIR)/$(MANFILE)
	$(MUTE)rm -f $(DESTDIR)$(PNGDIR)/16x16/apps/nsnake.png
	$(MUTE)rm -f $(DESTDIR)$(PNGDIR)/32x32/apps/nsnake.png
	$(MUTE)rm -f $(DESTDIR)$(XPMDIR)/nsnake.xpm
	$(MUTE)rm -f $(DESTDIR)$(DESKTOPDIR)/nsnake.desktop

$(EXE): $(OBJECTS) $(ENGINE_OBJECTS) $(COMMANDER_OBJECTS)
	# Linking...
	$(MUTE)$(CXX) $(OBJECTS) $(ENGINE_OBJECTS) $(COMMANDER_OBJECTS) -o bin/$(EXE) $(LIBSDIR) $(LDFLAGS)

src/%.o: src/%.cpp
	# Compiling $<...
	$(MUTE)$(CXX) $(CXXFLAGS) $(CDEBUG) $< -c -o $@ $(DEFINES) $(INCLUDESDIR)

dist: clean-all $(DISTDIR).tar.gz

# This creates a tarball with all the files
# versioned by GIT.
$(DISTDIR).tar.gz: $(DISTDIR)
	$(MUTE)tar czf $(DISTDIR).tar.gz $(DISTDIR)
	$(MUTE)rm -rf $(DISTDIR)
	$(MUTE)cp $(DISTDIR).tar.gz ..
	$(MUTE)rm -f $(DISTDIR).tar.gz
	# Created ../$(DISTDIR).tar.gz!

# This copies all the source code files into a
# subdirectory called $(DISTDIR).
#
# It uses `git ls-files` to create the directory
# tree and copy everything to their respective
# places.
#
$(DISTDIR):
	# Compressing source code...
	$(MUTE)mkdir -p $(DISTDIR)
	-$(MUTE)git ls-files | xargs -L 1 dirname | sed -e 's|^|$(DISTDIR)/|' | xargs -L 1 mkdir -p
	-$(MUTE)git ls-files | sed -e 's|\(.*\)|\0 $(DISTDIR)/\0|' | xargs -L 1 cp
	-$(MUTE)rm -f $(DISTDIR)/.gitignore

run: all
	# Running...
	$(MUTE)./bin/$(EXE)

clean:
	# Cleaning object files...
	$(MUTE)rm $(VTAG) -f $(OBJECTS)
	$(MUTE)rm $(VTAG) -f bin/$(EXE)

clean-all: clean
	# Cleaning dependency object files...
	$(MUTE)rm $(VTAG) -f $(ENGINE_OBJECTS) $(COMMANDER_OBJECTS)

dirs:
	$(MUTE)mkdir -p bin

doc:
	# Generating documentation...
	$(MUTE)doxygen Doxyfile

docclean:
	# Removing documentation...
	-$(MUTE)rm $(VTAG) -rf doc/html

.PHONY: clean clean-all dirs doc docclean uninstall

# Engine stuff
$(ENGINE_DIR)/%.o: $(ENGINE_DIR)/%.cpp
	# Compiling $<...
	$(MUTE)$(CXX) $(CXXFLAGS) $(CDEBUG) $< -c -o $@ $(DEFINES) $(INCLUDESDIR)

# Commander stuff
$(COMMANDER_DIR)/%.o: $(COMMANDER_DIR)/%.c
	# Compiling $<...
	$(MUTE)$(CC) $(COMMANDER_CFLAGS) $< -c -o $@

