#include <Engine/Helpers/String.hpp>

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
		elems.push_back(item);

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

