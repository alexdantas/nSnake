# nsnake Makefile
# (2013) Alexandre Dantas (kure) <eu@alexdantas.net>

SHELL    = /bin/bash
DATE     = $(shell date "+%B%Y")

PACKAGE  = nsnake
VERSION  = 2.0
EXE		 = nsnake

CXXFLAGS = -Wall -Wextra -g
LDFLAGS  = -lncurses -lcdk
INCLUDE  =

LIBS_CXXFILES = $(shell find src/lib -maxdepth 1 -type f -name '*.cpp')

DEFINES  = -DVERSION=\""$(VERSION)"\" \
           -DPACKAGE=\""$(PACKAGE)"\" \
           -DDATE=\""$(DATE)"\"

###################################################################
########## Do NOT edit this
CXX = g++

# All C++ file sources of the projects
CXXFILES   = $(shell find src -maxdepth 1 -type f -name '*.cpp')
CXXOBJECTS = $(CXXFILES:.cpp=.o)

SOURCES    = $(CXXFILES)
OBJECTS    = $(CXXOBJECTS)

LIBS_CXXOBJECTS = $(LIBS_CXXFILES:.cpp=.o)

LIBS_SOURCES = $(LIBS_CXXFILES)
LIBS_OBJECTS = $(LIBS_CXXOBJECTS)

ifdef V
MUTE =
VTAG = -v
else
MUTE = @
endif
###################################################################

# Don't try to understand this

# BUILD is initially undefined
ifndef BUILD

# max equals 256 x's
sixteen := x x x x x x x x x x x x x x x x
MAX := $(foreach x,$(sixteen),$(sixteen))

# T estimates how many targets we are building by replacing BUILD with
# a special string
T := $(shell $(MAKE) -nrRf $(firstword $(MAKEFILE_LIST)) $(MAKECMDGOALS) \
            BUILD="COUNTTHIS" | grep -c "COUNTTHIS")

# N is the number of pending targets in base 1, well in fact, base x
# :-)
N := $(wordlist 1,$T,$(MAX))

# auto-decrementing counter that returns the number of pending targets
# in base 10
counter = $(words $N)$(eval N := $(wordlist 2,$(words $N),$N))

# BUILD is now defined to show the progress, this also avoids
# redefining T in loop
BUILD = @echo $(counter) of $(T)
endif
###################################################################

all: dirs $(EXE)
	# Build successful!

$(EXE): $(OBJECTS) $(LIBS_OBJECTS)
	# Linking...
	$(MUTE)$(CXX) $(OBJECTS) $(LIBS_OBJECTS) -o $(EXE) $(LDFLAGS)

src/%.o: src/%.cpp
	# Compiling $<...
	$(MUTE)$(CXX) $(CXXFLAGS) $(INCLUDE) $< -c -o $@
	$(BUILD)

src/%.o: src/%.c
	# Compiling $<...
	$(MUTE)$(CC) $(CFLAGS) $(INCLUDE) $< -c -o $@

run: all
	$(MUTE)./$(EXE)

clean:
	# Cleaning...
	-$(MUTE)rm -f $(EXE) $(OBJECTS)

libclean: clean
	# Cleaning libs...
	-$(MUTE)rm -f $(LIBS_OBJECTS)

# Assure directories exist
dirs:
	$(MUTE)mkdir -p src/lib

doc:
	# Generating documentation...
	$(MUTE)doxygen Doxyfile

docclean:
	# Removing documentation...
	-$(MUTE)rm $(VTAG) -rf $(LDOC)/html

.PHONY: clean doc docclean

