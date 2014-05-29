#include <Config/INI.hpp>

#include <cstring>				// strncpy()

INI::INI():
	ini(NULL)
{ }
INI::~INI()
{
	this->free();
}

bool INI::load(std::string file)
{
	if (this->ini)
		this->free();

	this->ini = iniparser_load(file.c_str());

	return (this->ini ?
	        true :
	        false);
}
void INI::create()
{
	if (this->ini)
		this->free();

	this->ini = dictionary_new(0);
}
void INI::free()
{
	if (this->ini)
	{
		iniparser_freedict(this->ini);
		this->ini = NULL;
	}
}
bool INI::get(std::string where, bool default_value)
{
	int test = -666;

	test = iniparser_getboolean(this->ini, where.c_str(), test);

	if (test == -666)
		return default_value;

	return ((test == 1) ?
	        true:
	        false);
}
int INI::get(std::string where, int default_value)
{
	return iniparser_getint(this->ini, where.c_str(), default_value);
}
unsigned int INI::get(std::string where, unsigned int default_value)
{
	return (unsigned int)iniparser_getint(this->ini, where.c_str(), default_value);
}
double INI::get(std::string where, double default_value)
{
	return iniparser_getdouble(this->ini, where.c_str(), default_value);
}
std::string INI::get(std::string where, const char* default_value)
{
	// All this because I can't convert `const char*` to `char*`
	size_t len = strlen(default_value);

	char s[len+1];
	strncpy(s, default_value, len);

	// Finally!
	return iniparser_getstring(this->ini, where.c_str(), s);
}
std::string INI::get(std::string where, std::string default_value)
{
	// All this because I can't convert `const char*` to `char*`
	size_t len = default_value.size();

	char s[len+1];
	strncpy(s, default_value.c_str(), len);

	// Finally!
	return iniparser_getstring(this->ini, where.c_str(), s);
}
void INI::set(std::string what, std::string value)
{
	size_t pos = what.find(':');
	if (pos != std::string::npos)
	{
		// User's adding a section - like "section:key".
		// Let's make sure it exists
		std::string section = what.substr(0, pos);

		if (iniparser_find_entry(this->ini, section.c_str()) == 0)
		{
			// Doesn't exist - creating...
			this->set(section, "");
		}
	}

	iniparser_set(this->ini, what.c_str(), value.c_str());
}
void INI::unset(std::string what)
{
	iniparser_unset(this->ini, what.c_str());
}
void INI::save(std::string file)
{
	FILE* fp = fopen(file.c_str(), "w");
	if (fp)
	{
		this->save(fp);
		fclose(fp);
	}
}
void INI::save(FILE* file)
{
	if (! file)
		return;

	iniparser_dump_ini(this->ini, file);
}

