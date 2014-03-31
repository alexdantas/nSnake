#ifndef CONFIGFILE_H_DEFINED
#define CONFIGFILE_H_DEFINED

#include <string>
#include <cstdio>                // FILE* fopen() fclose()

#include <confuse.h>			// libConfuse

/// Loads settings from a file with a specific configuration format.
///
/// ## Configuration File Format
///
/// Has `.conf` extension and supports pairs of  keys and values.
/// Like:
///
///     # comment to end-of-line
///     key = value
///     key2 = value2
///
/// Blank lines are allowed.
///
/// Keys are case-insensitive.
/// Values can be:
///
/// * Boolean: yes/no, YES/NO, true/false, TRUE/FALSE, 1/0
/// * Integer: 42 (decimal), 042 (octal), 0x42 (hex)
/// * Double: 1.12321, 543.92502
/// * String: any sequence of characters to the end of line
///
/// ## Usage
///
///     ConfigFile file;
///     if (! file.load("filename.conf"))
///         // Woops, something bad happened.
///         // File doensn't exist, whatever
///     bool var = file.get("boolKeyName", true);
///     int otherVar = ConfigFile.get("intKeyName", 2);
///     std::string another = ConfigFile.get("stringKeyName", "4");
///
class CONFIGFILE
{
public:
	ConfigFile();
	virtual ~ConfigFile();

	/// Loads and parses #file.
	///
	/// @return If we've successfuly loaded the file.
	bool load(std::string file);

	/// Creates a blank ConfigFile file in memory.
	///
	/// @see save()
	void create();

	/// Cleans up internal allocated memory.
	///
	/// @note It is safe to call this function many times,
	///       and it's automatically called at the destructor
	///       so call it only if you want to release the
	///       memory early.
	///
	void free();

	/// Gets a boolean value located at #where inside the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	bool get(std::string where, bool default_value);

	/// Gets an integer value located at #where inside the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	int get(std::string where, int default_value);

	/// Gets an unsigned integer value located at #where inside
	/// the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	unsigned int get(std::string where, unsigned int default_value);

	/// Gets a double value located at #where inside the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	double get(std::string where, double default_value);

	/// Gets a string value located at #where inside the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	std::string get(std::string where, const char* default_value);

	/// Gets a string value located at #where inside the file.
	/// Check the class documentation for it's format.
	///
	/// @return The value or #default_value if it doesn't exist,
	///         the file itself isn't there or something happened.
	std::string get(std::string where, std::string default_value);

	/// Sets #what to #value.
	/// If it exists, will get overriden. If not, creates it.
	/// @see save()
	void set(std::string what, std::string value);

	/// Removes #what entry from internal ConfigFile contents.
	/// @see save()
	void unset(std::string what);

	/// Saves all internal ConfigFile key-values to #file.
	///
	/// @note No comments are preserved when saving!
	void save(std::string file);

	/// C-style saving of the internal ConfigFile key-values.
	///
	/// @note The advantage of it is that you can specify
	///       stdout or stderr.
	/// @note It checks for nullptr pointers, don't worry.
	/// @note Again, no comments are preserved when saving!
	void save(FILE* file);

private:

	/// ConfigFileparser's internal data structure.
	cfg_t* ConfigFile;
};

#endif //CONFIGFILE_H_DEFINED

