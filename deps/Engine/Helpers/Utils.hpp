#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

#include <string>
#include <vector>
#include <sys/stat.h> // mkdir() and off_t
#include <sstream>

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
		///
		/// @param percent A float between 0 and 1
		///
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

		/// Forcibly removes file within #path.
		/// @note It doesn't work with directories.
		void rm_f(std::string path);

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

		/// Returns the component of a pathname (file name and extension).
		///
		/// - If we have "/path/to/something.txt" it returns "something.txt"
		/// - If we have "something.txt" it returns "something.txt"
		///
		/// @note It auto-detects the separator for Windows ('\')
		///       and UNIX-based systems ('/')
		///
		/// Thanks to this huge list of OS-specific defines:
		/// http://sourceforge.net/p/predef/wiki/OperatingSystems/
		std::string basename(std::string path);

		/// Returns the full pathname up to the last component.
		///
		/// - If we have "/path/to/something.txt" it returns "/path/to"
		/// - If we have "something.txt" it returns ""
		///
		std::string dropBasename(std::string path);

		/// Returns the extension of a file.
		///
		/// @note It doesn't return the dot.
		///
		/// - If we have "/path/to/file.txt" it returns "txt"
		/// - If we have "filename.DLL" it returns "DLL"
		/// - If we have ".hidden" it returns ""
		/// - If we have "none" it returns ""
		///
		/// @note Works with full paths or single filenames.
		std::string extension(std::string path);

		/// Returns the filename without it's extension.
		///
		/// @note Works with full paths or single filenames.
		std::string dropExtension(std::string path);
	};


	/// Utilities to encode and decode from the Base64 format.
	namespace Base64
	{
		/// Transforms #str into a Base64 equivalent.
		std::string encode(std::string str);

		/// Transforms a Base64-encoded #str into it's regular string equivalent.
		std::string decode(std::string const& s);
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

