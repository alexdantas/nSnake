# nSnake Makefile (2011-2012)  Alexandre Dantas (kure) <alex.dantas92@gmail.com>
#
# Makefile Commandlines:
#  V       Print all commands as they are called.
#          To turn on for the current make, add 'V=1' on the
#          commandline.
#          To turn on permanently, uncomment the line specified below
#  DESTDIR Installs the package on a custom root directory (other than /)
#          For example 'DESTDIR=~/'.
#  PREFIX  Installs the package on a custom directory (overwrites root)
#  CFLAGS  Changes the C flags used on compilation
#  CDEBUG  If you wish to build on debug mode, add 'CDEBUG=-g'
#
# Makefile Targets:
#  all        Compiles the package
#  run        Compiles and runs the program
#  install    Installs the package
#  clean      Removes the binary and the resulting object files
#  uninstall  Uninstalls the package
#  dist       Creates the source code 'tarball' for distribution
#  doc        Generates the documentation with doxygen
#  docclean   Removes the documentation

# Uncomment line below to tun on verbose mode permanently
#V	= 1;

SHELL	= /bin/sh

# General Info
PACKAGE = nsnake
VERSION = 1.5
DATE	= `date '+%b%Y'`

# Local source code information
LBIN    = bin
LOBJ    = obj
LDOC    = doc
LSRC    = src
LFILES  = BUGS ChangeLog COPYING Doxyfile INSTALL Makefile README TODO

# Install
DESTDIR =
PREFIX	= $(DESTDIR)/usr

EXEC_PREFIX = $(PREFIX)
DATAROOTDIR = $(PREFIX)/share
MANROOT     = $(DATAROOTDIR)/man

BINDIR	  = $(EXEC_PREFIX)/games
MANDIR    = $(MANROOT)/man6
MANNUMBER = 6

# Package configuration files
SCORE_FILE = nsnake.scores
SCOREDIR   = $(DESTDIR)/var/games
SCORE_PATH = $(SCOREDIR)/$(SCORE_FILE)

# Compiling information
CC          = gcc
EXE         = nsnake
CDEBUG	    =
CFLAGS	    = $(CDEBUG) -Wall -Wextra -O2
LIBS	    = -lncurses
INCLUDESDIR =
LIBSDIR     =
OBJ         = $(LOBJ)/fruit.o      \
              $(LOBJ)/main.o       \
              $(LOBJ)/player.o     \
              $(LOBJ)/nsnake.o     \
              $(LOBJ)/engine.o     \
              $(LOBJ)/hscores.o    \
              $(LOBJ)/arguments.o
MANFILE     = $(PACKAGE).$(MANNUMBER)
MANPAGE     = $(LDOC)/man/$(MANFILE)

DEFINES	= -DVERSION=\"$(VERSION)\"        \
          -DDATE=\"$(DATE)\"              \
          -DSCORE_PATH=\"$(SCORE_PATH)\"

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

# Make targets
all: dirs $(EXE)
	# Build successful!

install: all
	# Installing...
	$(MUTE)install -d $(SCOREDIR)
	$(MUTE)install -d --mode=755 $(BINDIR)
	$(MUTE)install --mode=755 $(LBIN)/$(EXE) $(BINDIR)
	-$(MUTE)chown root:games $(BINDIR)/$(EXE)
	-$(MUTE)chmod u+s $(BINDIR)/$(EXE)
	$(MUTE)install -d $(MANDIR)
	$(MUTE)install $(MANPAGE) $(MANDIR)
	@echo
	# $(PACKAGE) successfuly installed!

uninstall:
	# Uninstalling...
	$(MUTE)rm -f $(BINDIR)/$(EXE)

purge: uninstall
	# Purging configuration files...
	$(MUTE)rm -f $(SCORE_PATH)
	$(MUTE)rm -f $(MANDIR)/$(MANFILE)

$(EXE): $(OBJ)
	# Linking...
	$(MUTE)$(CC) $(OBJ) -o $(LBIN)/$(EXE) $(LIBSDIR) $(LIBS)

$(LOBJ)/%.o: $(LSRC)/%.c
	# Compiling $<...
	$(MUTE)$(CC) $(CFLAGS) $< -c -o $@ $(DEFINES) $(INCLUDESDIR)

dist: $(DISTDIR).tar.gz

$(DISTDIR).tar.gz: $(DISTDIR)
	$(MUTE)tar czf $(DISTDIR).tar.gz $(DISTDIR)
	$(MUTE)rm -rf $(DISTDIR)
	$(MUTE)cp $(DISTDIR).tar.gz ..
	$(MUTE)rm -f $(DISTDIR).tar.gz

$(DISTDIR):
	$(MUTE)mkdir -p $(DISTDIR)/$(LSRC) $(DISTDIR)/$(LDOC)
	$(MUTE)mkdir -p $(DISTDIR)/$(LBIN) $(DISTDIR)/$(LOBJ)
	-$(MUTE)cp $(LFILES) -t $(DISTDIR)
	-$(MUTE)cp -r $(LSRC)/* $(DISTDIR)/$(LSRC)
	-$(MUTE)cp -r $(LBIN)/* $(DISTDIR)/$(LBIN)
	-$(MUTE)cp -r $(LDOC)/* $(DISTDIR)/$(LDOC)

# Phony targets
dirs:
	-mkdir -p $(LOBJ) $(LBIN)

run: all
	# Running...
	$(MUTE)./$(LBIN)/$(EXE)

clean:
	# Cleaning files...
	$(MUTE)rm $(VTAG) -f $(LOBJ)/*.o
	$(MUTE)rm $(VTAG) -f $(LBIN)/*

doc:
	# Generating documentation...
	$(MUTE)doxygen Doxyfile

docclean:
	# Removing documentation...
	-$(MUTE)rm $(VTAG) -rf $(LDOC)/html

.PHONY: clean doc docclean uninstall

#------------------------------------------------------------------------------
