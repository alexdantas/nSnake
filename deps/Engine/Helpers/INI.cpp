#include <Engine/Helpers/INI.hpp>
#include <Engine/Helpers/String.hpp>

void INI::Level::addGroup(std::string name)
{
	name = Utils::String::trim(name);

	// When we call `this->sections[name]` we already create
	// a new one if it doesn't exist.
	//
	// The only way we'll know we just made a new one is if
	// it's parent is NULL.
	//
	// Since the only one allowed to be like this is the top
	// (and the top always exist), this is the way we check
	// for newly-created group.
	if (this->sections[name].parent != NULL)
		return;

	// Setting it's parent as ourselves
	this->sections[name].parent = this;
	this->sections[name].depth  = this->depth + 1;

	// Registering it on ourselves
	this->ordered_sections.push_back(this->sections.find(name));
}
void INI::Level::addKey(std::string name, std::string value)
{
	name  = Utils::String::trim(name);
	value = Utils::String::trim(value);

	std::pair<Level::ValueMap::const_iterator, bool> res =
		this->values.insert(std::make_pair(name, value));

	if (!res.second)
	{
		// Found other key with this name,
		// let's overwrite its value
		this->values[name] = value;
		return;
	}
	this->ordered_values.push_back(res.first);
}

void INI::Parser::raise_error(std::string msg)
{
	std::string buffer = ("Error '" +
	                      msg +
	                      "' on line #" +
	                      Utils::String::toString(this->lines));

	throw std::runtime_error(buffer);
}

INI::Parser::Parser() :
	lines(0)
{
	this->create();
}

INI::Parser::Parser(std::string filename) :
	input_file(filename.c_str()),
	input(&input_file),
	lines(0)
{
	if (! input)
		throw std::runtime_error("Failed to open file: " + filename);

	parse(this->top_level);
}

INI::Parser::Parser(std::istream& stream) :
	input(&stream),
	lines(0)
{
	parse(this->top_level);
}

INI::Level& INI::Parser::top()
{
	return this->top_level;
}

void INI::Parser::dump(std::ostream& stream)
{
	dump(stream, top(), "");
}

void INI::Parser::parseLevelLine(std::string& sname, size_t& depth)
{
	depth = 0;

	for (; depth < line_.length(); ++depth)
		if (line_[depth] != '[') break;

	sname = line_.substr(depth, line_.length() - 2*depth);
}

void INI::Parser::parse(INI::Level& level)
{
	while (std::getline(*input, line_))
	{
		++lines;

		if (line_[0] == '#' || line_[0] == ';') continue;

		line_ = Utils::String::trim(line_);

		if (line_.empty()) continue;

		if (line_[0] == '[')
		{
			size_t depth;

			std::string sname;

			parseLevelLine(sname, depth);

			INI::Level* level_current = NULL;
			INI::Level* parent        = &level;

			if (depth > level.depth + 1)
				raise_error("section with wrong depth");

			if (level.depth == depth - 1)
				level_current = &level.sections[sname];

			else
			{
				level_current = level.parent;

				size_t n = (level.depth - depth);

				for (size_t i = 0; i < n; ++i)
					level_current = level_current->parent;

				parent = level_current;

				level_current = &level_current->sections[sname];

			}

			if (level_current->depth != 0)
				raise_error("duplicate section name on the same level");

			if (!level_current->parent)
			{
				level_current->depth  = depth;
				level_current->parent = parent;
			}

			parent->ordered_sections.push_back(parent->sections.find(sname));

			parse(*level_current);
		}
		else
		{
			// Not a group - found a key-value pair, like:
			// `something = other_something`

			size_t pos = line_.find('=');

			if (pos == std::string::npos)
				raise_error("no '=' found");

			std::string key   = line_.substr(0, pos);
			std::string value = line_.substr((pos + 1), (line_.length()-pos-1));

			level.addKey(key, value);
		}
	}
}

void INI::Parser::dump(std::ostream& s, const INI::Level& l, const std::string& sname)
{
	if (!sname.empty())
		s << '\n';

	for (size_t i = 0; i < l.depth; ++i)
		s << '[';

	if (!sname.empty())
		s << sname;

	for (size_t i = 0; i < l.depth; ++i)
		s << ']';

	if (!sname.empty())
		s << std::endl;

	for (INI::Level::Values::const_iterator it = l.ordered_values.begin();
	     it != l.ordered_values.end();
	     ++it)
		s << (*it)->first << '=' << (*it)->second << std::endl;

	for (INI::Level::Sections::const_iterator it = l.ordered_sections.begin();
	     it != l.ordered_sections.end();
	     ++it)
	{
		assert((*it)->second.depth == l.depth+1);

		dump(s, (*it)->second, (*it)->first);
	}
}

void INI::Parser::saveAs(std::string filename)
{
	std::ofstream file_out(filename.c_str());
	if (!file_out)
		throw std::runtime_error(std::string("Couldn't open '" + filename + "'"));

	this->dump(file_out);
}

void INI::Parser::create()
{
	this->top_level = Level();
}

