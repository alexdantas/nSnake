#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

#include <string>
#include <vector>
#include <sys/stat.h> // mkdir() and off_t
#include <sstream>
#include <stdexcept>

/// Random useful things accumulated over the years.
///
namespace Utils
{
	/// Better random number generator.
	namespace Random
	{
		/// Must be called before any of those.
		void seed();

		/// Random number between `min` and `max`.
		int between(int min, int max);

		/// Random boolean.
		bool boolean();

		/// Random boolean with chance of #percent.
		/// @note Precision up to 2 decimal digits.
		bool booleanWithChance(float percent);
	};

	namespace Time
	{
		/// Stops execution for #delay microseconds.
		void delay_ms(int delay);
	};

	/// File I/O and Operational System's utilities.
	///
	/// Note, I'm using several POSIX functions.
	/// So the following functions surely aren't
	/// portable to Windows.
	/// Other systems are kinda unpredictable.
	///
	namespace File
	{
		/// Tells if #path exists.
		///
		/// @note It could be a file, directory or whatever.
		bool exists(std::string path);

		/// Returns the file size of #path in bytes.
		///
		/// @return It's size or -1 if it doesn't exist
		///         (or something strange happened).
		off_t size(std::string path);

		/// Creates #path directory hierarchy recursively,
		/// just like UNIX command `mkdir -p`.
		void mkdir_p(std::string path);

		/// Removes recursively all files within directory
		/// at #path, just like UNIX command `rm -rf`.
		void rm_rf(std::string path);

		/// Creates empty file #path.
		///
		/// @note If file already exists, will
		///       erase everything inside!
		/// @return If we could create the file at all.
		bool create(std::string path);

		/// Writes #contents to #path.
		///
		/// @note If #path doesn't exist, creates it.
		/// @note If #path exist, overwrites everything on it.
		void write(std::string path, std::string contents);

		/// Tells if #path is a directory.
		///
		/// @note Returns false also if something wrong happened.
		bool isDirectory(std::string path);

		/// Tells if #path is a regular file
		/// (not a directory, socket, FIFO device or whatever).
		///
		/// @note Returns false also if something wrong happened.
		bool isFile(std::string path);

		/// Lists all files withing #path.
		///
		/// @note The returned vecor is not ordered and
		///       all file names contain the full #path
		///       before them.
		std::vector<std::string> ls(std::string path);

		/// Gets the full path of the home directory for
		/// the user running this program.
		///
		/// @return The path or an empty string.
		/// @note We guarantee that the path has a trailing '/'.
		std::string getHome();

		/// Gets the user name of the person running this program.
		std::string getUser();
	};

	namespace String
	{
		/// Converts from any type that supports the
		/// << operator to std::string.
		///
		/// @note Due to templates, we must place the definition
		///       on the header.
		/// @note Call it like `Utils::String::toString(var)`
		template <typename T>
		inline std::string toString(T const& x)
		{
			std::ostringstream o;
			if (!(o << x))
				throw std::runtime_error("Utils::String::toString");

			return o.str();
		}

		/// Converts from std::string to any type
		/// that supports the << operator.
		///
		/// @note Due to templates, we must place the definition
		///       on the header.
		/// @note Call it like `Utils::String::convert(string, var)`
		template <typename T>
		inline void convert(std::string const& s, T& x,
		                    bool failIfLeftOverChars=true)
		{
			std::istringstream i(s);
			char c;
			if (!(i >> x) || (failIfLeftOverChars && i.get(c)))
				throw std::runtime_error("Utils::String::convert");
		}

		/// An easier way to call previous function.
		/// @note Call it like `Utils::String::to<int>(string)`
		template <typename T>
		inline T to(std::string const& s,
		            bool failIfLeftOverChars=true)
		{
			T x;
			convert(s, x, failIfLeftOverChars);
			return x;
		}

		/// Returns the last character on the string.
		///
		/// @note C++11 compatibility!
		/// @thanks http://stackoverflow.com/a/4884579
		char back(std::string& str);

		/// Returns the first character on the string.
		///
		/// @note C++11 compatibility!
		char front(std::string& str);

		/// Destructively removes the last character of #str.
		/// @note C++11 compatibility!
		/// @thanks http://stackoverflow.com/a/2311025
		void pop_back(std::string* str);

		/// Non-destructively removes the last character of #str.
		/// (returns a new string)
		std::string pop_back(std::string& str);

		/// Removes all space on the left of `str`.
		std::string& ltrim(std::string &str);

		/// Removes all space on the right of `str`.
		std::string& rtrim(std::string& str);

		/// Removes all space on both sides of `str`.
		std::string& trim(std::string& str);

		/// Splits `str` according to `delimt`.
		///
		/// @return A vector with two elements - string
		///         before and string after `delimit`.
		///
		std::vector<std::string> split(const std::string& str, char delim);
	};
};

// Useful #defines collected over the years.

/// Deletes #pointer if valid, invalidating it after.
#define SAFE_DELETE(pointer) \
	{ \
		if (pointer) \
		{ \
			delete(pointer); \
			pointer = NULL; \
		} \
	}

/// Tells the compiler we're not using #thing as an argument.
/// Don't forget ';' after it!
#define UNUSED(thing) \
	{ \
		(void)(thing); \
	  \
	}

#endif //UTILS_H_DEFINED

