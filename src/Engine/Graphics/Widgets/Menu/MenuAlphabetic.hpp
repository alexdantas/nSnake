#ifndef MENUALPHABETIC_H_DEFINED
#define MENUALPHABETIC_H_DEFINED

#include <Engine/Graphics/Widgets/Menu.hpp>

/// Menu on which its items are always sorted alphabetically.
///
/// It has special navigation keys: by pressing a letter
///                                 it will jump straight to
///                                 the first occurrence of it.
class MenuAlphabetic: public Menu
{
public:
	MenuAlphabetic(int x, int y, int width, int height);

	void add(MenuItem* item);
	void handleInput();

	/// Selects a random item right AFTER the first
	/// blank one.
	void goRandom();
};

#endif //MENUALPHABETIC_H_DEFINED

