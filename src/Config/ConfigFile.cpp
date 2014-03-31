#include <Config/ConfigFile.hpp>

#include <cstring>				// strncpy()

ConfigFile::ConfigFile():
	configfile(nullptr)
{ }
ConfigFile::~ConfigFile()
{
	this->free();
}

bool ConfigFile::load(std::string file)
{
	if (this->configfile)
		this->free();

	this->configfile = configfileparser_load(file.c_str());

	return (this->configfile ?
	        true :
	        false);
}
void ConfigFile::create()
{
	if (this->configfile)
		this->free();

	this->configfile = dictionary_new(0);
}
void ConfigFile::free()
{
	if (this->configfile)
	{
		configfileparser_freedict(this->configfile);
		this->configfile = nullptr;
	}
}
bool ConfigFile::get(std::string where, bool default_value)
{
	int test = -666;

	test = configfileparser_getboolean(this->configfile, where.c_str(), test);

	if (test == -666)
		return default_value;

	return ((test == 1) ?
	        true:
	        false);
}
int ConfigFile::get(std::string where, int default_value)
{
	return configfileparser_getint(this->configfile, where.c_str(), default_value);
}
unsigned int ConfigFile::get(std::string where, unsigned int default_value)
{
	return (unsigned int)configfileparser_getint(this->configfile, where.c_str(), default_value);
}
double ConfigFile::get(std::string where, double default_value)
{
	return configfileparser_getdouble(this->configfile, where.c_str(), default_value);
}
std::string ConfigFile::get(std::string where, const char* default_value)
{
	// All this because I can't convert `const char*` to `char*`
	size_t len = strlen(default_value);

	char s[len+1];
	strncpy(s, default_value, len);

	// Finally!
	return configfileparser_getstring(this->configfile, where.c_str(), s);
}
std::string ConfigFile::get(std::string where, std::string default_value)
{
	// All this because I can't convert `const char*` to `char*`
	size_t len = default_value.size();

	char s[len+1];
	strncpy(s, default_value.c_str(), len);

	// Finally!
	return configfileparser_getstring(this->configfile, where.c_str(), s);
}
void ConfigFile::set(std::string what, std::string value)
{
	size_t pos = what.find(':');
	if (pos != std::string::npos)
	{
		// User's adding a section - like "section:key".
		// Let's make sure it exists
		std::string section = what.substr(0, pos);

		if (configfileparser_find_entry(this->configfile, section.c_str()) == 0)
		{
			// Doesn't exist - creating...
			this->set(section, "");
		}
	}

	configfileparser_set(this->configfile, what.c_str(), value.c_str());
}
void ConfigFile::unset(std::string what)
{
	configfileparser_unset(this->configfile, what.c_str());
}
void ConfigFile::save(std::string file)
{
	FILE* fp = fopen(file.c_str(), "w");
	if (fp)
	{
		this->save(fp);
		fclose(fp);
	}
}
void ConfigFile::save(FILE* file)
{
	if (! file)
		return;

	configfileparser_dump_configfile(this->configfile, file);
}

