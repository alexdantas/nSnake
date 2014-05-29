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

#include <Misc/Utils.hpp>

#include <sstream>     // sstream
#include <algorithm>   // find_if
#include <ctime>       // time()
#include <unistd.h>    // usleep()
#include <sys/types.h> // opendir(), readdir()
#include <dirent.h>    // readdir()
#include <iostream>	   // ofstream
#include <fstream>	   // ofstream
#include <stdlib.h>	   // system()

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

//  ____  _   _     ____
// | |_  | | | |   | |_
// |_|   |_| |_|__ |_|__

bool Utils::File::exists(std::string path)
{
	return (Utils::File::size(path) != -1);
}
off_t Utils::File::size(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) < 0)
		return -1;

	return s.st_size;
}
void Utils::File::mkdir_p(std::string path)
{
	std::string tmp(path);

	if (Utils::String::back(tmp) == '/')
		tmp[tmp.size() - 1] = '\0';

	for (std::string::iterator p = tmp.begin();
	     (*p) != '\0';
	     p++)
	{
		if (*p == '/')
		{
			*p = '\0';
			mkdir(tmp.c_str(), S_IRWXU);
			*p = '/';
		}
	}
	mkdir(tmp.c_str(), S_IRWXU);
}
void Utils::File::rm_rf(std::string path)
{
	if (! Utils::File::isDirectory(path))
		return;

	// Another BIG UGY HACK
	//
	// Since my program's already non-portable
	// (POSIX systems only) and I don't have the
	// time nor means to either use Boost or
	// implement my own file functions, I'll
	// have to do a big hack.
	//
	// It's ugly, please ignore this function.
	//
	// I can't believe you're still reading.
	// Please don't continue from here.
	//
	// I've dishounored my family.
	// So ashamed of myself.
	//
	//
	// Next thing you know we're throwing gotos
	// everywhere.
	//
	//
	//
	// Still reading?
	//
	// ...OK, I've warned you

	std::string command("rm -rf " + path);

	system(command.c_str());
}
bool Utils::File::create(std::string path)
{
	FILE* fp = fopen(path.c_str(), "w");
	if (! fp)
		return false;

	fclose(fp);
	return true;
}
void Utils::File::write(std::string path, std::string contents)
{
	std::ofstream file;
	file.open(path.c_str()); // if it was C++11 we could've used std::string
	file << contents;
}
bool Utils::File::isDirectory(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) < 0)
		return false;

	return ((S_ISDIR(s.st_mode))?
	        true:
	        false);
}
bool Utils::File::isFile(std::string path)
{
	struct stat s;

	if (stat(path.c_str(), &s) < 0)
		return false;

	return ((S_ISREG(s.st_mode))?
	        true:
	        false);
}
std::vector<std::string> Utils::File::ls(std::string path)
{
	std::vector<std::string> v;

	if (! Utils::File::isDirectory(path))
		return v;

	// Opening directory
	DIR* dir;

	if (! (dir = opendir(path.c_str())))
		return v;

	// Assuring 'path' ends with '/'
	if (Utils::String::back(path) != '/')
		path.push_back('/');

	// Getting contents
	struct dirent* ent;

	while ((ent = readdir(dir)))
	{
		std::string s(path + ent->d_name);

		// Skipping obvious '.' and '..' dirs
		if ((s == (path + '.')) || (s == (path + "..")))
			continue;

		v.push_back(s);
	}
	closedir(dir);

	return v;
}
std::string Utils::File::getHome()
{
	if (! getenv("HOME"))
		return "";

	std::string s(getenv("HOME"));
	if (Utils::String::back(s) != '/')
		s.push_back('/');

	return s;
}
std::string Utils::File::getUser()
{
	std::string s = Utils::File::getHome();
	if (s.empty())
		return "";

	// Removing trailing '/'
	Utils::String::pop_back(&s);

	// Getting everything after other '/'
	size_t pos = s.rfind('/');

	if (pos == std::string::npos) // woah, wtf
		return "";

	return s.substr(pos + 1);
}

//  __  _____  ___   _   _      __
// ( (`  | |  | |_) | | | |\ | / /`_
// _)_)  |_|  |_| \ |_| |_| \| \_\_/

char Utils::String::back(std::string& str)
{
	// Using the reverse iterator
	return *(str.rbegin());
}

char Utils::String::front(std::string& str)
{
	return *(str.begin());
}

void Utils::String::pop_back(std::string* str)
{
	if (str->size() > 0)
		str->resize(str->size() - 1);
}

std::string Utils::String::pop_back(std::string& str)
{
	return (str.substr(0, str.size() - 1));
}

std::string& Utils::String::ltrim(std::string &str)
{
	// Using some std black magic. Taken from here:
	// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

	// Here we create a predicate to be compared.
	// (In other words, "an element that `isspace`")
	std::pointer_to_unary_function<int, int> function = std::ptr_fun<int, int>(std::isspace);

	// This returns the first element that's not a space.
	//
	// It will go inside `str` looking for the first
	// element that matches a predicate.

	std::string::iterator it = std::find_if(str.begin(),
	                                        str.end(),

	                                        // And here we negate the predicate
	                                        // ("an element that's NOT `isspace`")
	                                        std::not1(function));

	// And here we erase everything up to it.
	str.erase(str.begin(),
	          it);

	return str;
}

std::string& Utils::String::rtrim(std::string& str)
{
	// More std magic. Sorry for the mess.
	// Please check method above (`ltrim`).

	str.erase(std::find_if(str.rbegin(),
	                       str.rend(),
	                       std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
	          str.end());

	return str;
}

std::string& Utils::String::trim(std::string& str)
{
	return Utils::String::ltrim(Utils::String::rtrim(str));
}

std::vector<std::string> Utils::String::split(const std::string& str, char delim)
{
	std::stringstream ss(str);      // "buffer"
	std::string item;               // current thing
	std::vector<std::string> elems; // all things

	while (std::getline(ss, item, delim))
		elems.push_back(Utils::String::trim(item));

	return elems;
}

