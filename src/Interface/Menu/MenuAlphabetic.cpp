#include <Interface/Menu/MenuAlphabetic.hpp>
#include <Misc/Utils.hpp>

#include <algorithm>

MenuAlphabetic::MenuAlphabetic(int x, int y, int width, int height):
	Menu(x, y, width, height)
{ }

// Local function that compares two Menu Items
bool menuItemLess(const MenuItem* x, const MenuItem* y)
{
	return Utils::String::caseInsensitiveSmallerString(x->label, y->label);
}

void MenuAlphabetic::add(MenuItem* item)
{
	Menu::add(item);

	std::sort(this->item.begin(),
	          this->item.end(),
	          menuItemLess);
}
void MenuAlphabetic::handleInput()
{
	Menu::handleInput();


}

