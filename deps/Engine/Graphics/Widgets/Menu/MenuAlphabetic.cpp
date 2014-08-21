#include <Engine/Graphics/Widgets/Menu/MenuAlphabetic.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/InputManager.hpp>

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

	// This is a very specific behavior, read with attention.
	//
	// If the menu has no blank items,
	// we're going to sort it normally.
	//
	// If it has a single blank item, we'll sort the entire menu
	// starting from the item AFTER the blank one.

	std::vector<MenuItem*>::iterator firstItem = this->item.begin();

	// So here we look for a blank item

	for (size_t i = 0; i < (this->item.size()); i++)
		if (this->item[i] == NULL)
		{
			firstItem += i; // Pointing to the current items' position
			break;
		}

	// And start sorting from AFTER it
	std::sort(firstItem + 1,
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
		// See specific behavior on `MenuAlphabetic::add`
		//
		// Start looking only after the first Blank item.
		int startingIndex = 0;

		for (size_t i = 0; i < this->item.size(); i++)
			if (this->item[i] == NULL)
			{
				startingIndex = i;
				break;
			}

		// And only make possible to jump on items AFTER
		// the blank one
		for (size_t i = startingIndex + 1; i < this->item.size(); i++)
			if (std::tolower(this->item[i]->label[0]) == key)
			{
				this->currentIndex = i;
				this->current = this->item[i];
				break;
			}
	}
}
void MenuAlphabetic::goRandom()
{
	// See specific behavior on `MenuAlphabetic::add`
	//
	// Start looking only after the first Blank item.
	int startingIndex = 0;

	for (size_t i = 0; i < this->item.size(); i++)
		if (this->item[i] == NULL)
		{
			startingIndex = i;
			break;
		}


	this->currentIndex = Utils::Random::between(startingIndex + 1, this->item.size() - 1);
	this->current      = this->item[this->currentIndex];
}

