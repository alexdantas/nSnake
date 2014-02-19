#ifndef ARRAY2D_H_DEFINED
#define ARRAY2D_H_DEFINED

#include <vector>
#include <iostream> // size_t

/// Two-dimensional array.
///
/// This class tries to be as simple as C's matrix handling
/// and very extensible, with `std::vector`.
///
/// ## Note
///
/// Array's contents must be a type/class that has a
/// constructor with no arguments.
///
/// So if you create your `MyClass` and want a 2D array of it,
/// make sure `MyClass()` has a constructor that receive no
/// arguments.
///
template<class T>
class Array2D
{
public:
	/// Creates a 2D array with `width` and `height`.
	Array2D(int width, int height);
	virtual ~Array2D() { };

	/// Returns element at `x` `y`.
	T at(int x, int y)
	{
		return contents[x][y];
	}

	void set(int x, int y, const T& value)
	{
		contents[x][y] = value;
	}

	/// Width size of the array.
	size_t width();

	/// Height size of the array.
	size_t height();

private:
	/// Internal storage of elements.
	std::vector<std::vector<T> > contents;
};

// Damn you templates!
//
// I need to leave the function definitions on the header
// since we need to tell the compiler to create any possible
// templates for each type called on the whole program.

template <class T>
Array2D<T>::Array2D(int width, int height)
{
	contents.resize(width);

	for (int i = 0; i < width; i++)
		contents[i].resize(height);
}

template <class T>
size_t Array2D<T>::width()
{
	return contents.size();
}

template <class T>
size_t Array2D<T>::height()
{
	return contents[0].size();
}

#endif //ARRAY2D_H_DEFINED

