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

	// TODO: make it keep pointing to the previous item
	//       instead of jumping to the first
	this->goFirst();
}

void MenuAlphabetic::handleInput()
{
	Menu::handleInput();

	// This is very ugly...

	char key = std::tolower(InputManager::pressedKey);

	if (key >= 'a' && key <= 'z')
	{
		for (size_t i = 0; i < this->item.size(); i++)
			if (std::tolower(this->item[i]->label[0]) == key)
			{
				this->currentIndex = i;
				this->current = this->item[i];
				break;
			}
	}
}

