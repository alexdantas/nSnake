#include <Config/YAMLFile.hpp>
#include <fstream>              // std::ofstream
#include <exception>

YAMLFile::YAMLFile()
{ }
YAMLFile::~YAMLFile()
{
}

void YAMLFile::load(std::string file)
{
	this->yaml_node = YAML::LoadFile(file);
}
void YAMLFile::create()
{
	this->yaml_node = YAML::Node();
}
bool YAMLFile::get(std::string out, std::string in, bool default_value)
{
	if ((this->yaml_node[out]     == NULL) ||
	    (this->yaml_node[out][in] == NULL))
		return default_value;

	return this->yaml_node[out][in].as<bool>();
}
int YAMLFile::get(std::string out, std::string in, int default_value)
{
	if ((this->yaml_node[out]     == NULL) ||
	    (this->yaml_node[out][in] == NULL))
		return default_value;

	return this->yaml_node[out][in].as<int>();
}
char YAMLFile::get(std::string out, std::string in, char default_value)
{
	if ((this->yaml_node[out]     == NULL) ||
	    (this->yaml_node[out][in] == NULL))
		return default_value;

	return this->yaml_node[out][in].as<char>();
}
double YAMLFile::get(std::string out, std::string in, double default_value)
{
	if ((this->yaml_node[out]     == NULL) ||
	    (this->yaml_node[out][in] == NULL))
		return default_value;

	return this->yaml_node[out][in].as<double>();
}
std::string YAMLFile::get(std::string out, std::string in, const char* default_value)
{
	return get(out, in, std::string(default_value));
}
std::string YAMLFile::get(std::string out, std::string in, std::string default_value)
{
	if ((this->yaml_node[out]     == NULL) ||
	    (this->yaml_node[out][in] == NULL))
		return default_value;

	return this->yaml_node[out][in].as<std::string>();
}
void YAMLFile::set(std::string out, std::string in, bool value)
{
	this->yaml_node[out][in] = value;
}
void YAMLFile::set(std::string out, std::string in, int value)
{
	this->yaml_node[out][in] = value;
}
void YAMLFile::set(std::string out, std::string in, double value)
{
	this->yaml_node[out][in] = value;
}
void YAMLFile::set(std::string out, std::string in, char value)
{
	this->yaml_node[out][in] = value;
}
void YAMLFile::set(std::string out, std::string in, std::string value)
{
	this->yaml_node[out][in] = value;
}

void YAMLFile::save(std::string file)
{
	std::ofstream file_out(file.c_str());
	if (!file_out)
		throw std::runtime_error(std::string("Couldn't open '" + file + "'"));

	file_out << this->yaml_node;
}

