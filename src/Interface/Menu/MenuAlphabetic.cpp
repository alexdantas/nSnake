#include <Interface/Menu/MenuAlphabetic.hpp>
#include <Misc/Utils.hpp>
#include <Flow/InputManager.hpp>

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

	// This is very ugly...

	if (InputManager::pressedKey >= 'a' && InputManager::pressedKey <= 'z')
	{
		for (size_t i = 0; i < this->item.size(); i++)
			if (this->item[i]->label[0] == InputManager::pressedKey)
			{
				this->currentIndex = i;
				this->current = this->item[i];
			}
	}
}

