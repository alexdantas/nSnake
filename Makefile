#------------------------------------------------------------------------------
#    servw Makefile
#
#    (2011)  Alexandre Dantas (kure) <alex.dantas92@gmail.com>
#
#    Commandlines:
#
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
#
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

# Uncomment to tun on the verbose mode for every command
#V  = 1;

SHELL   = /$(LBIN)/sh

#-------General Info------------------------------------------------------------
PACKAGE = servw
VERSION = 0.5
DATE    = Dec2011

#-------Local build information-------------------------------------------------
LBIN    = bin
LOBJ    = obj
LDOC    = doc
LSRC    = src
LFILES  = ChangeLog COPYING Doxyfile INSTALL Makefile README TODO

#-------Install-----------------------------------------------------------------
DESTDIR =
PREFIX  = $(DESTDIR)/usr/local

DATAROOTDIR  = $(PREFIX)/share
EXECC_PREFIX = $(PREFIX)

BINDIR  = $(EXECC_PREFIX)/$(LBIN)
DOCDIR  = $(DATAROOTDIR)/$(LDOC)/$(PACKAGE)

#-------Configure---------------------------------------------------------------
LOCALSTATEDIR =

#-------Compile-----------------------------------------------------------------
CC        = gcc
EXEC      = $(PACKAGE)
CDEBUG    =
CFLAGS    = $(CDEBUG) -Wall -Wextra -O2
LINKFLAGS = 
LIBS      = 
OBJ       = $(LOBJ)/server.o $(LOBJ)/main.o $(LOBJ)/client.o
DEFINES   = -DVERSION=\"$(VERSION)\" \
            -DDATE=\"$(DATE)\"       \
            -DPACKAGE=\"$(PACKAGE)\"
INSTALL   = install -s

#-------Distribute--------------------------------------------------------------
DISTDIR = $(PACKAGE)-$(VERSION)
TARNAME = $(DISTDIR).tar.gz

#-------Verbose Mode Checker----------------------------------------------------
ifdef V
MUTE =
VTAG = -v
else
MUTE = @
endif

#-------Standard Makes----------------------------------------------------------

all: $(EXEC)
	@echo "* Build completed!"

install: all
	@echo "* Installing..."
	$(MUTE)install -d --mode=755 $(BINDIR)
	$(MUTE)install --mode=755 $(LBIN)/$(EXEC) $(BINDIR)
	@echo "**********************************"
	@echo "* Program successfuly installed! *"
	@echo "**********************************"

uninstall:
	@echo "* Uninstalling..."
	$(MUTE)rm -f $(BINDIR)/$(EXEC)

# To create the executable file
$(EXEC): $(OBJ)
	@echo "* Linking..."
	$(MUTE)$(CC) $(OBJ) $(LINKFLAGS) -o $(LBIN)/$(EXEC) $(LIBS)

# All the object files
$(LOBJ)/%.o: $(LSRC)/%.c
	@echo "* Compiling $<..."
	$(MUTE)$(CC) $(CFLAGS) $< -c -o $@ $(DEFINES)

#-------Custom Makes-----------------------------------------------------------

# Make the 'tarball'
dist: $(TARNAME)

$(TARNAME): $(DISTDIR)
	$(MUTE)tar czf $(TARNAME) $(DISTDIR)
	$(MUTE)rm -rf $(DISTDIR)
	$(MUTE)cp $(DISTDIR).tar.gz ..
	$(MUTE)rm -f $(DISTDIR).tar.gz

$(DISTDIR):
	$(MUTE)mkdir -p $(DISTDIR)/$(LSRC) $(DISTDIR)/$(LDOC)
	$(MUTE)mkdir -p $(DISTDIR)/$(LBIN) $(DISTDIR)/$(LOBJ)
	-$(MUTE)cp $(LFILES) -t $(DISTDIR)
	-$(MUTE)cp -r $(LSRC)/* $(DISTDIR)/$(LSRC)
	-$(MUTE)cp -r $(LDOC)/* $(DISTDIR)/$(LDOC)

# Creates a new directory above with a new specified version.
newversion: dist
	$(MUTE)tar $(VTAG) -xzf $(TARNAME)

#-------Phonys-----------------------------------------------------------------
run: all
	@echo "* Running..."
	$(MUTE)./$(LBIN)/$(EXEC)

clean:
	@echo "* Cleaning..."
	$(MUTE)rm $(VTAG) -f $(LOBJ)/*.o
	$(MUTE)rm $(VTAG) -f $(LBIN)/*

dox:
	@echo "* Documenting..."
	$(MUTE)doxygen Doxyfile
	-$(MUTE)ln $(VTAG) -s $(LDOC)/html/index.html $(LDOC)/$(PACKAGE)\ documentation

doxclean:
	@echo "* Removing documentation..."
	-$(MUTE)rm $(VTAG) -rf $(LDOC)/html
	-$(MUTE)rm $(VTAG) -rf $(LDOC)/latex
	-$(MUTE)rm $(VTAG) -rf $(LDOC)/$(PACKAGE)\ documentation

debug: clean
	$(MUTE)make all CFLAGS=-g
	@echo "* Running Debugger..."
	$(MUTE)gdb ./$(LBIN)/$(EXEC)


.PHONY: clean dox doxclean uninstall

#------------------------------------------------------------------------------

