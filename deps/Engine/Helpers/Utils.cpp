//  _________________________________________________________
// |       _        __    ___   _      _   _      __         |
// |      \ \    / / /\  | |_) | |\ | | | | |\ | / /`_       |
// |       \_\/\/ /_/--\ |_| \ |_| \| |_| |_| \| \_\_/       |
// |                                                         |
// | Adventurers, beware...                                  |
// |                                                         |
// | * The following file has lots of methods accumulated    |
// |   over the years.                                       |
// | * There isn't too much cohesion between them, so try    |
// |   to understand them individually.                      |
// |                                                         |
// |                                                         |
// | * They're mostly poor-coded, sorry 'bout that.          |
// |                                                         |
// `---------------------------------------------------------'

#include <Engine/Helpers/Utils.hpp>
#include <Engine/Helpers/String.hpp>

#include <sstream>     // sstream
#include <algorithm>   // find_if
#include <ctime>       // time()
#include <unistd.h>    // usleep()
#include <algorithm>

// C++11 compatibility
// I wish I could use those:
// #include <utility>
// #include <random>

//  ___    __    _      ___   ___   _
// | |_)  / /\  | |\ | | | \ / / \ | |\/|
// |_| \ /_/--\ |_| \| |_|_/ \_\_/ |_|  |

void Utils::Random::seed()
{
	// Poor choice for random numbers, I know
	// I wish I could use C++11's random generators...
	srand(time(NULL));
}

int Utils::Random::between(int min, int max)
{
	if (min > max)
		std::swap(min, max);

	return (rand() % (max - min + 1) + min);
}

bool Utils::Random::boolean()
{
	// If a random number between 0 and 9 is even
	int random_int = Utils::Random::between(0, 9);

	return ((random_int % 2) == 0);
}

bool Utils::Random::booleanWithChance(float percent)
{
	int x = Utils::Random::between(0, 99);

	return (x < (percent * 100));
}

// _____  _   _      ____
//  | |  | | | |\/| | |_
//  |_|  |_| |_|  | |_|__

void Utils::Time::delay_ms(int delay)
{
	usleep((useconds_t)delay * 100);
}

