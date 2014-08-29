#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

#include <string>
#include <vector>

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

