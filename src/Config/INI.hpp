#ifndef INI_H_DEFINED
#define INI_H_DEFINED

#include <cassert>
#include <map>
#include <list>
#include <stdexcept>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

/// Simple module that contains everything needed to load
/// and parse a file with the INI configuration format.
///
/// It basically is an INI::Parser, which uses several INI::Level
/// according to the contents of the file.
///
/// ## Usage
///
/// For an INI file like this:
///
///     key       = value
///     [group]
///     other_key = other_value
///     [[nested_group]]
///     final_key = final_value
///
/// You'd run this module like this:
///
///     // Loads and parses immediately the file
///     INI::Parser parser("path/to/file.ini");
///     // Returns "value"
///     parser["key"];
///     // Returns "other_value"
///     parser("group")["other_key"];
///     // Returns "final_value"
///     parser("group")("nested_group")["final_key"];
///
/// ## Credits
///
/// This module is a modified version of `ini-parser`,
/// originally made by Poordeveloper.
///
/// - Homepage:    https://github.com/Poordeveloper/ini-parser
/// - DevHomepage: https://github.com/Poordeveloper
///
namespace INI {

	/// Contains a "scope" of the INI file.
	///
	/// Suppose an INI file like this:
	///
	///     key=value
	///     [group]
	///     key=value
	///     [[group2]]
	///     key=value
	///     [[[group3]]
	///     key=value
	///
	/// We have four Levels, `group`, `group2` and `group3`
	/// and the top level, which contains all other levels.
	///
	/// You access keys with the `[]` operator and child groups
	/// with the `()` operator.
	/// So, for the example above:
	///
	///     parser.top()("group")("group2")("group3")["key"] == "value"
	///
	struct Level
	{
		/// Create the topmost Level.
		Level() :
			parent(NULL),
			depth(0)
		{ }

		/// Create a level with parent #p.
		Level(Level* p) :
			parent(p),
			depth(0)
		{ }

		/// The parent Level of this one.
		/// NULL for the topmost.
		Level* parent;

		/// Counter of how many nested levels this one is.
		size_t depth;

		typedef std::map<std::string, std::string>    ValueMap;
		typedef std::map<std::string, Level>          SectionMap;
		typedef std::list<ValueMap::const_iterator>   Values;
		typedef std::list<SectionMap::const_iterator> Sections;

		/// All the key values inside this Level.
		/// So for an INI like this:
		///
		///     [group]
		///     key=value
		///
		/// This would return "value":
		///
		///     level.values["key"]
		///
		ValueMap values;

		/// All the Levels inside this Level.
		/// So for an INI like this:
		///
		///     [group]
		///     [group2]
		///     key=value
		///
		/// This would return "value":
		///
		///     level.sections["group2"].values["key"]
		///
		SectionMap sections;

		/// All values in the original order of the INI file.
		Values ordered_values;

		/// All Sections in the original order of the INI file.
		Sections ordered_sections;

		/// Access a key within this Level.
		const std::string& operator[](const std::string& name)
		{
			return this->values[name];
		}

		/// Access another Level within this Level.
		Level& operator()(const std::string& name)
		{
			return this->sections[name];
		}

		/// Creates a new child group with #name.
		/// @note If it already exists, do nothing.
		/// @note Inside this method we trim `name`
		///       of spaces and tabs.
		void addGroup(std::string name);

		/// Creates a new key #name with #value.
		///
		/// @note If the key already exists will overwrite it's value.
		/// @note Inside this method we trim `name` and `value`
		///       of spaces and tabs.
		void addKey(std::string name, std::string value);
	};

	/// Loads, reads and parses the contents of an INI file (or string).
	///
	class Parser
	{

	public:
		/// Creates a blank new INI file.
		///
		/// @see Parser::create()
		Parser();

		/// Load and parse #filename.
		Parser(std::string filename);

		/// Parse a stream.
		/// It can be used to parse strings from memory.
		Parser(std::istream& stream);

		/// Outputs the contents of the INI file to #stream.
		///
		/// It dumps a valid INI file, according to this
		/// parsers modifications.
		///
		void dump(std::ostream& stream);

		/// Returns the top level of this INI file.
		/// You can then access all it's keys and nested groups
		/// with the Level methods.
		///
		/// @see Level
		Level& top();

		/// Shortcut to access a key within the top level.
		const std::string& operator[](const std::string& name)
		{
			return this->top()[name];
		}

		/// Shortcut to access a Level within the top level.
		Level& operator()(const std::string& name)
		{
			return this->top()(name);
		}

		/// Creates a blank INI registry.
		///
		/// It resets itself, allowing you to create brand new
		/// INI files from scratch.
		///
		/// @see Level::addKey()
		/// @see level::addGroup()
		///
		/// @note It drops everything that may already have
		///       been loaded.
		void create();

		/// Save all the internal INI contents on a file with #filename.
		void saveAs(std::string filename);

	private:
		void dump(std::ostream& s, const Level& l, const std::string& sname);

		void parse(Level& l);

		/// Parses a line that defines a Level.
		///
		/// Lines like this:
		///
		///     [group]
		///     [[group]]
		///
		/// And so on...
		///
		void parseLevelLine(std::string& sname, size_t& depth);

		/// Throws an exception with error message #msg.
		/// It also contains the current line number.
		void raise_error(std::string msg);

		Level top_level;

		std::ifstream input_file;

		std::istream* input;

		std::string line_;

		/// Counter of how many lines we've parsed.
		size_t lines;
	};
}

#endif // INI_H_DEFINED

