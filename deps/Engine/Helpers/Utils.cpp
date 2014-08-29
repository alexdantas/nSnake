//  _________________________________________________________
// |       _        __    ___   _      _   _      __         |
// |      \ \    / / /\  | |_) | |\ | | | | |\ | / /`_       |
// |       \_\/\/ /_/--\ |_| \ |_| \| |_| |_| \| \_\_/       |
// |                                                         |
// | Adventurers, beware...                                  |
// |                                                         |
// | * The following file has lots of methods accumulated    |
// |   over the years.                                       |
// | * There isn't too much cohesion between them, so try    |
// |   to understand them individually.                      |
// |                                                         |
// |                                                         |
// | * They're mostly poor-coded, sorry 'bout that.          |
// |                                                         |
// `---------------------------------------------------------'

#include <Engine/Helpers/Utils.hpp>
#include <Engine/Helpers/String.hpp>

#include <sstream>     // sstream
#include <algorithm>   // find_if
#include <ctime>       // time()
#include <unistd.h>    // usleep()
#include <algorithm>

// C++11 compatibility
// I wish I could use those:
// #include <utility>
// #include <random>

//  ___    __    _      ___   ___   _
// | |_)  / /\  | |\ | | | \ / / \ | |\/|
// |_| \ /_/--\ |_| \| |_|_/ \_\_/ |_|  |

void Utils::Random::seed()
{
	// Poor choice for random numbers, I know
	// I wish I could use C++11's random generators...
	srand(time(NULL));
}

int Utils::Random::between(int min, int max)
{
	if (min > max)
		std::swap(min, max);

	return (rand() % (max - min + 1) + min);
}

bool Utils::Random::boolean()
{
	// If a random number between 0 and 9 is even
	int random_int = Utils::Random::between(0, 9);

	return ((random_int % 2) == 0);
}

bool Utils::Random::booleanWithChance(float percent)
{
	int x = Utils::Random::between(0, 99);

	return (x < (percent * 100));
}

// _____  _   _      ____
//  | |  | | | |\/| | |_
//  |_|  |_| |_|  | |_|__

void Utils::Time::delay_ms(int delay)
{
	usleep((useconds_t)delay * 100);
}

/**
 * These Base64 functions are modified versions of
 * René Nyffenegger's `base64.cpp` and `base64.h`.
 *
 * Copyright (C) 2004-2008 René Nyffenegger
 *
 * This source code is provided 'as-is', without any express or implied
 * warranty. In no event will the author be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this source code must not be misrepresented; you must not
 *    claim that you wrote the original source code. If you use this source code
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original source code.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 */

// All allowed characters inside the Base64 domain.
static const std::string base64_chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

// Tells if some character #c belongs to the Base64 charset.
static inline bool isBase64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}


std::string Utils::Base64::encode(std::string str)
{
	// Getting the raw bytes we'll encode
	// Dark C++ casting magic here.
	unsigned char const* bytes_to_encode = reinterpret_cast<const unsigned char*>(str.c_str());
	unsigned int string_size = str.size();

	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (string_size--)
	{
		char_array_3[i++] = *(bytes_to_encode++);

		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';
	}
	return ret;
}

std::string Utils::Base64::decode(std::string const& encoded_string)
{
	int string_size = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (string_size-- && ( encoded_string[in_] != '=') && isBase64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_]; in_++;

		if (i ==4)
		{
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	return ret;
}


