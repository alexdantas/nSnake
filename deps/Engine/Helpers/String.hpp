#ifndef STRING_H_DEFINED
#define STRING_H_DEFINED

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

namespace Utils
{
	/// Helpers for Strings.
	///
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
		std::string ltrim(const std::string &str);

		/// Removes all space on the right of `str`.
		std::string rtrim(const std::string& str);

		/// Removes all space on both sides of `str`.
		std::string trim(const std::string& str);

		/// Splits `str` according to `delimt`.
		///
		/// @return A vector of strings, without the delimiter.
		///
		std::vector<std::string> split(const std::string& str, char delim);

		/// Tells if some character is smaller than other
		/// independently of it's case (upcase or smallcase).
		///
		bool caseInsensitiveSmallerChar(const char x, const char y);

		/// Tells if a whole string is smaller than other
		/// independently of it's case (upcase or smallcase).
		///
		/// This allows sorting a vector of strings case insensitively.
		/// in a very easy way. Look at it:
		///
		///     std::sort(vector.begin(),
		///               vector.end(),
		///               Utils::String::caseInsensitiveSmallerString);
		///
		/// Thanks to: Philip Nicoletti
		///
		bool caseInsensitiveSmallerString(const std::string &a, const std::string &b);
	}
}

#endif //STRING_H_DEFINED

