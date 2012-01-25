#------------------------------------------------------------------------------
#    nSnake Makefile
#
#    (2011)  Alexandre Dantas (kure) <alex.dantas92@gmail.com>
#
#    Commandlines:
#        V       Verbose mode, off by default.
#                To turn on for the current command, add 'V=1' on the
#                commandline OR uncomment the line specified below for
#                verbose on every command.
#        DESTDIR Installs the package on a custom root directory (other than /)
#        PREFIX  Installs the package on a custom directory (overwrites root)
#        CC      Changes the C flags used on compilation
#        CDEBUG  If you wish to build on debug mode, add CDEBUG=-g
#
#    Targets:
#        all:        Compiles the binary
#        clean:      Removes the binary and the *.o files
#        install:    Installs the package
#        uninstall:  Uninstalls the package
#        dist:       Creates the 'tarball' for distribution
#
#        newversion: Creates a new directory above with a new specified
#                    version. For example: 'make newversion VERSION=1.2'
#        run:        Compiles and runs the binary
#        dox:        Generates doxygen documentation
#        doxclean:   Removes the doxygen documentation
#------------------------------------------------------------------------------

#	Uncomment to tun on the verbose mode 'permanently'
#V	= 1;

SHELL	= /bin/sh

#-------General Info------------------------------------------------------------
PACKAGE = nsnake
VERSION = 1.5
DATE	= Jan2012

#-------Local build information-------------------------------------------------
LBIN    = bin
LOBJ    = obj
LDOC    = doc
LSRC    = src
LFILES  = BUGS ChangeLog COPYING Doxyfile INSTALL Makefile README TODO

#-------Install-----------------------------------------------------------------
DESTDIR =
PREFIX	= $(DESTDIR)/usr/local

EXEC_PREFIX = $(PREFIX)
DATAROOTDIR = $(PREFIX)/share
MANDIR  = $(DATAROOTDIR)/man

BINDIR	= $(EXEC_PREFIX)/$(LBIN)
MAN6DIR = $(MANDIR)/man6

#-------Configure---------------------------------------------------------------

SCORE_FILE = nsnake.scores
SCOREDIR   = $(DESTDIR)/var/games
SCORE_PATH = $(SCOREDIR)/$(SCORE_FILE)

#-------Compile-----------------------------------------------------------------
CC	    = gcc
EXE	    = nsnake
CDEBUG	=
CFLAGS	= $(CDEBUG) -Wall -Wextra -O2
LIBS	= -lncurses
OBJ	    = $(LOBJ)/fruit.o      \
          $(LOBJ)/main.o       \
          $(LOBJ)/player.o     \
          $(LOBJ)/nsnake.o     \
          $(LOBJ)/engine.o     \
          $(LOBJ)/hscores.o    \
          $(LOBJ)/arguments.o
MANFILE = $(PACKAGE).6.gz
MANPAGE = $(LDOC)/$(MANFILE)

DEFINES	= -DVERSION=\"$(VERSION)\"        \
          -DDATE=\"$(DATE)\"              \
          -DSCORE_PATH=\"$(SCORE_PATH)\"

#-------Distribute--------------------------------------------------------------
TARNAME = $(PACKAGE)
DISTDIR = $(TARNAME)-$(VERSION)

#-------Verbose Mode------------------------------------------------------------

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

#-------Standard Makes----------------------------------------------------------

all: $(EXE)
	@echo "* Ready to Play!"

install: all
	@echo "* Installing..."
#Agora nsnake roda como root:games
	$(MUTE)install -d $(SCOREDIR)
	$(MUTE)install -d --mode=755 $(BINDIR)
	$(MUTE)install --mode=755 $(LBIN)/$(EXE) $(BINDIR)
	$(ROOT)$(MUTE)chown root:games $(BINDIR)/$(EXE)
	$(ROOT)$(MUTE)chmod u+s $(BINDIR)/$(EXE)
	$(MUTE)install -d $(MAN6DIR)
	$(MUTE)install $(MANPAGE) $(MAN6DIR)
	@echo
	@echo "* $(PACKAGE) successfuly installed!"

uninstall:
	@echo "* Uninstalling..."
	$(MUTE)rm -f $(BINDIR)/$(EXE)

#	Also removes the configuration files
purge: uninstall
	@echo "* Purging configuration files..."
	$(MUTE)rm -f $(SCORE_PATH)
	$(MUTE)rm -f $(MAN6DIR)/$(MANFILE)

#	To make the executable file
$(EXE): $(OBJ)
	@echo "* Linking..."
	$(MUTE)$(CC) $(OBJ) -o $(LBIN)/$(EXE) $(LIBS)

#	All the object files
$(LOBJ)/%.o: $(LSRC)/%.c
	@echo "* Compiling $<..."
	$(MUTE)$(CC) $(CFLAGS) $< -c -o $@ $(DEFINES)

#	Make the 'tarball'
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

# Creates a new directory above with a new specified version.
# Remember to change the version manually on the new
# Makefile after doing this!
newversion: dist
	$(MUTE)tar $(VTAG) -xzf $(TARNAME)

#-------Phonys-----------------------------------------------------------------
run: all
	@echo "* Running..."
	$(MUTE)./$(LBIN)/$(EXE)

clean:
	@echo "* Cleaning..."
	$(MUTE)rm $(VTAG) -f $(LOBJ)/*.o
	$(MUTE)rm $(VTAG) -f $(LBIN)/*

dox:
	@echo "* Documenting..."
	$(MUTE)doxygen Doxyfile
	-$(MUTE)ln $(VTAG) -s html/index.html $(LDOC)/$(PACKAGE)\_doc

doxclean:
	@echo "* Removing documentation..."
	-$(MUTE)rm $(VTAG) -rf $(LDOC)/html $(LDOC)/$(PACKAGE)\_doc

.PHONY: clean doxy backup uninstall

#------------------------------------------------------------------------------
