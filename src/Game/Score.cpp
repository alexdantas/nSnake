#include <Game/Score.hpp>

#include <stdlib.h>	  // getenv()

Score::Score():
	name("player"),
	points(0),
	lines(0),
	level(0)
{
	// Will try to get current user's name, or empty string.
	if (getenv("USER"))
		name = getenv("USER");
}

