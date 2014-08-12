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
#include <algorithm>
#include <cstring>     // strchr()

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
void Utils::File::rm_f(std::string path)
{
	if (Utils::File::isDirectory(path))
		return;

	// This is ALSO another big hack.
	// God-dang it
	std::string command("rm -f " + path);

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
std::string Utils::File::basename(std::string path)
{
#if defined(_WIN32)  && !defined(__CYGWIN__)
	char separator = '\\';
#else
	char separator = '/';
#endif

	size_t position = path.rfind(separator);

	// Didn't find
	if (position == std::string::npos)
		return path;

	// Return from after the separator to the end
	return path.substr(position + 1);
}
std::string Utils::File::dropBasename(std::string path)
{
	std::string basename = Utils::File::basename(path);
	if (basename.empty())
		return path;

	size_t position = path.find(basename);

	if (position == std::string::npos)
		return "";

	// Return from start to before the separator
	return path.substr(0, position - 1);
}
std::string Utils::File::extension(std::string path)
{
	size_t position = path.rfind('.');

	if ((position == std::string::npos) || // Didn't find
	    (position == 0))                   // File name starts with a dot
		return "";

	// Return from after the dot to the end
	return path.substr(position + 1);
}
std::string Utils::File::dropExtension(std::string path)
{
	std::string extension = Utils::File::extension(path);
	if (extension.empty())
		return path;

	size_t position = path.find(extension);

	if (position == std::string::npos)
		return "";

	// Return from start to (and including) the dot
	return path.substr(0, position - 1);
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

const char trim_blanks[] = " \t\r\n"; // Characters to be removed

std::string Utils::String::ltrim(const std::string& str)
{
	size_t startpos = str.find_first_not_of(trim_blanks);

	// Found no blanks
	if (startpos == std::string::npos)
		return "";

	return str.substr(startpos);
}
std::string Utils::String::rtrim(const std::string& str)
{
	size_t endpos = str.find_last_not_of(trim_blanks);

	// Found no blanks
	if (endpos == std::string::npos)
		return "";

	return str.substr(0, endpos + 1);
}
std::string Utils::String::trim(const std::string& str)
{
	return (Utils::String::ltrim(
		        Utils::String::rtrim(
			        str)));
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

bool Utils::String::caseInsensitiveSmallerChar(const char x, const char y)
{
	return (std::tolower(x) < std::tolower(y));
}

bool Utils::String::caseInsensitiveSmallerString(const std::string &a, const std::string &b)
{
	return std::lexicographical_compare(a.begin(), a.end(),
	                                    b.begin(), b.end(),
	                                    Utils::String::caseInsensitiveSmallerChar);
}

/**
 * These Base64 functions are modified versions of
 * René Nyffenegger's `base64.cpp` and `base64.h`.
 *
 * Copyright (C) 2004-2008 René Nyffenegger
 *
 * This source code is provided 'as-is', without any express or implied
 * warranty. In no event will the author be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this source code must not be misrepresented; you must not
 *    claim that you wrote the original source code. If you use this source code
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original source code.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 */

// All allowed characters inside the Base64 domain.
static const std::string base64_chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

// Tells if some character #c belongs to the Base64 charset.
static inline bool isBase64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}


std::string Utils::Base64::encode(std::string str)
{
	// Getting the raw bytes we'll encode
	// Dark C++ casting magic here.
	unsigned char const* bytes_to_encode = reinterpret_cast<const unsigned char*>(str.c_str());
	unsigned int string_size = str.size();

	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (string_size--)
	{
		char_array_3[i++] = *(bytes_to_encode++);

		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';
	}
	return ret;
}

std::string Utils::Base64::decode(std::string const& encoded_string)
{
	int string_size = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (string_size-- && ( encoded_string[in_] != '=') && isBase64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_]; in_++;

		if (i ==4)
		{
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	return ret;
}


