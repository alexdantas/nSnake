#include <Engine/Helpers/File.hpp>
#include <Engine/Helpers/String.hpp>

#include <sys/types.h> // opendir(), readdir()
#include <dirent.h>    // readdir()
#include <stdlib.h>	   // system()
#include <stdio.h>     // fopen()
#include <iostream>	   // ofstream
#include <fstream>	   // ofstream

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

