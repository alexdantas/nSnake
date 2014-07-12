#ifndef YAML_H_DEFINED
#define YAML_H_DEFINED

#include <string>
#include <cstdio>                // FILE* fopen() fclose()
#include <yaml-cpp/yaml.h>

/// Loads settings from a file with the YAML standard.
///
/// ## YAML file
///
/// An YAML file format maps keys to values like this:
///
///     # Comment to the end of the line
///     outer_name:
///       key: value
///       other_key: other_value
///     array:
///       [1, 2, 3, 4]
///     list:
///       - first
///       - second
///     complex_list:
///       - name: first
///         id: 1
///       - name: second
///         id: 2
///
/// Blank lines are allowed.
///
///
/// ## Usage
///
///     YAML yaml;
///     yaml.load("filename.yml"))
///     bool var = yaml.get("outer_name", "inner_name", true);
///     int otherVar = yaml.get("outer", "inner", 2);
///     std::string another = yaml.get("out", in", "4");
///
class YAMLFile
{
public:
	YAMLFile();
	virtual ~YAMLFile();

	/// Loads and parses #file.
	///
	/// @return If we've successfuly loaded the file.
	void load(std::string file);

	/// Creates a blank ini file in memory.
	///
	/// @see save()
	void create();

	/// Gets a boolean value located at #where inside the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	bool get(std::string out, std::string in, bool default_value);

	/// Gets an integer value located at #where inside the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	int get(std::string out, std::string in, int default_value);

	/// Gets an unsigned integer value located at #where inside
	/// the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	char get(std::string out, std::string in, char default_value);

	/// Gets a double value located at #where inside the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	double get(std::string out, std::string in, double default_value);

	/// Gets a string value located at #where inside the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	std::string get(std::string out, std::string in, const char* default_value);

	/// Gets a string value located at #where inside the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	std::string get(std::string out, std::string in, std::string default_value);

	void set(std::string out, std::string in, bool value);
	void set(std::string out, std::string in, int value);
	void set(std::string out, std::string in, char value);
	void set(std::string out, std::string in, double value);
	void set(std::string out, std::string in, std::string value);

	/// Saves all internal ini key-values to #file.
	///
	/// @note No comments are preserved when saving!
	void save(std::string file);

private:

	/// yaml-cpp's internal data structure.
	YAML::Node yaml_node;
};

#endif //YAML_H_DEFINED

