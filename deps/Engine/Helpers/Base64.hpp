#ifndef BASE64_H_DEFINED
#define BASE64_H_DEFINED

#include <string>

namespace Utils
{
	/// Utilities to encode and decode from the Base64 format.
	namespace Base64
	{
		/// Transforms #str into a Base64 equivalent.
		std::string encode(std::string str);

		/// Transforms a Base64-encoded #str into its
		/// regular string equivalent.
		std::string decode(std::string const& s);
	}
}

#endif //BASE64_H_DEFINED

