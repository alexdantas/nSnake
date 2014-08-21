#include <Engine/Graphics/Widgets/Menu.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/InputManager.hpp>

Menu::Menu(int x, int y, int width, int height):
	current(NULL),
	currentIndex(0),
	x(x),
	y(y),
	width(width),
	height(height),
	selected(false),
	selectedItem(NULL)
{ }
Menu::~Menu()
{
	for (unsigned int i = 0; i < (this->item.size()); i++)
		SAFE_DELETE(this->item[i]);

	this->item.clear();
}
void Menu::add(MenuItem* item)
{
	this->item.push_back(item);

	// Means we're the first item being added!
	if (this->item.size() == 1)
	{
		this->current      = this->item.back();
		this->currentIndex = this->item.size() - 1;
	}

	// Means we're the first non-nil item being added!
	unsigned int i = 0;
	for (i = 0; i < (this->item.size()); i++)
		if (this->item[i])
			break;

	this->current      = this->item[i];
	this->currentIndex = i;
}
void Menu::addBlank()
{
	this->item.push_back(NULL);
}
void Menu::removeByID(int id)
{
	std::vector<MenuItem*>::iterator it = this->item.begin();

	while (it != this->item.end())
	{
		if (!(*it))
			++it;

		if ((*it)->id == id)
		{
			if (this->current == *it)
			{
				this->goNext();
				this->currentIndex--;
			}

			this->item.erase(it);
			return;
		}
		else
			++it;
	}
}
void Menu::removeByLabel(std::string label)
{
	std::vector<MenuItem*>::iterator it = this->item.begin();

	while (it != this->item.end())
	{
		if (!(*it))
			++it;

		if ((*it)->label == label)
		{
			if (this->current == *it)
			{
				this->goNext();
				this->currentIndex--;
			}

			this->item.erase(it);
			return;
		}
		else
			++it;
	}
}
void Menu::draw(Window* window)
{
	// If we have more Items than we can draw, we need to
	// ask the user to scroll.

	// So these are the indexes of the items we'll actually
	// show on the screen.
	unsigned int draw_begin = 0;
	unsigned int draw_end   = this->item.size();

	if (this->height < (int)this->item.size())
	{
		if ((int)this->currentIndex <= this->height/2)
		{
			draw_end = this->height - 1;
		}
		else if ((int)this->currentIndex < ((int)this->item.size() - this->height/2) - 1)
		{
			draw_begin = this->currentIndex - this->height/2;
			draw_end   = this->currentIndex + this->height/2;
		}
		else
		{
			draw_begin = this->item.size() - this->height;
		}
	}

	// `i` is the index of the item to draw
	for (unsigned int curitem = draw_begin, yoffset = 0;
	     curitem < draw_end;
	     curitem++, yoffset++)
	{
		// Menu is big and needs scrolling,
		// we need to show (more) on the top
		if ((curitem == draw_begin) && (curitem != 0))
		{
			window->print("(more)",
			              this->x + this->width/2 - 3,
			              this->y + yoffset,
			              Colors::pair("white", "default"));
			continue;
		}
		// Menu is big and needs scrolling,
		// we need to show (more) on the end
		if ((curitem == draw_end - 1) && (curitem != this->item.size() - 1))
		{
			window->print("(more)",
			              this->x + this->width/2 - 3,
			              this->y + yoffset + 1,
			              Colors::pair("white", "default"));
			continue;
		}

		// <rant>
		// THIS IS VERY UGLY
		// HOW CAN I LAY DOWN A CLASS HIERARCHY
		// AND OVERRIDE PARENT FUNCTIONS ON CHILD CLASSES
		// IF WHEN I HAVE A PARENT POINTER I CANT LET THE
		// COMPILER DECIDE WETHER TO CALL PARENT OR CHILD
		// FUNCTIONS?
		// </rant>

//		MenuItemCheckbox* pCheckbox = dynamic_cast<MenuItemCheckbox*>a

		// Blank Menu Item, will show horizontal line
		if (! this->item[curitem])
		{
			for (int j = 0; j < (this->width); j++)
				window->printChar(((EngineGlobals::Screen::fancy_borders) ?
				                   ACS_HLINE :
				                   '-'),
				                  this->x + j,
				                  this->y + yoffset,
				                  Colors::pair("white", "default"));
		}
		else
		{
			this->item[curitem]->draw(window,
			                          this->x,
			                          this->y + yoffset,
			                          this->width,

			                          // Highlighting current item if
			                          // it's the current.
			                          (this->item[curitem] == this->current));
		}
	}
}
void Menu::handleInput()
{
	if (InputManager::noKeyPressed())
		return;

	if (InputManager::isPressed("down") || // user-defined
	    InputManager::isPressed(KEY_DOWN) ||
	    InputManager::isPressed('\t'))
		this->goNext();

	else if (InputManager::isPressed("up") ||
	         InputManager::isPressed(KEY_UP) ||
	         InputManager::isPressed(KEY_BTAB))
		this->goPrevious();

	else if (InputManager::isPressed(KEY_HOME) ||
	         InputManager::isPressed(KEY_PPAGE))
		this->goFirst();

	else if (InputManager::isPressed(KEY_END) ||
	         InputManager::isPressed(KEY_NPAGE))
		this->goLast();

	else if (InputManager::isPressed(KEY_ENTER) ||
	         InputManager::isPressed('\n'))
	{
		// Will only quit if the selected item is a simple
		// item or label - more complex ones doesn't quit.
		if (this->current->type == MenuItem::ITEM ||
			this->current->type == MenuItem::LABEL)
		{
			this->selected = true;
			this->selectedItem = this->current;
		}
		else
			this->current->handleInput();
	}
	else
	{
		if (this->current)
			this->current->handleInput();
	}
}
void Menu::goNext()
{
	// Empty element, piece of cake
	if (this->item.size() == 0)
		return;

	// Empty element, also piece of cake
	if (this->item.size() == 1)
	{
		this->current = *(this->item.begin());
		this->currentIndex = 0;
		return;
	}

	// Last element...
	// Well, if the last element is nil then we have
	// a problem.
	if (this->current == this->item.back())
	{
		// Assuming we're not nil, things will go smooth
		if (this->currentIndex == (this->item.size() - 1))
		{
			this->goFirst();
			return;
		}
	}

	this->currentIndex++;
	this->current = this->item[this->currentIndex];

	if (! this->current)
		this->goNext();
}
void Menu::goPrevious()
{
	if (this->item.size() == 0)
		return;

	if (this->item.size() == 1)
	{
		this->current = this->item.front();
		this->currentIndex = 0;
		return;
	}

	if (this->current == this->item.front())
	{
		if (this->currentIndex == 0)
		{
			this->goLast();
			return;
		}
	}

	this->currentIndex--;
	this->current = this->item[this->currentIndex];

	if (! this->current)
		this->goPrevious();
}
void Menu::goFirst()
{
	if (this->item.size() == 0)
		return;

	if (this->item.size() == 1)
	{
		this->current = this->item.front();
		this->currentIndex = 0;
		return;
	}

	this->current = this->item.front();
	this->currentIndex = 0;

	if (! this->current)
		this->goNext();
}
void Menu::goLast()
{
	if (this->item.size() == 0)
		return;

	if (this->item.size() == 1)
	{
		this->current = this->item.front();
		this->currentIndex = 0;
		return;
	}

	this->current = this->item.back();
	this->currentIndex = (this->item.size() - 1);

	if (! this->current)
		this->goPrevious();
}
void Menu::goRandom()
{
	if (this->item.size() == 0)
		return;

	this->currentIndex = Utils::Random::between(0, this->item.size() - 1);
	this->current      = this->item[this->currentIndex];
}
bool Menu::willQuit()
{
	// Will only quit if the user selected an item
	// and the item selected is valid.
	return (this->selected && this->selectedItem);
}
std::string Menu::currentLabel()
{
	if (! this->current)
		this->goNext();

	return (this->current->label);
}
int Menu::currentID()
{
	if (! this->current)
		this->goNext();

	return (this->current->id);
}
bool Menu::getBool(int id)
{
	for (unsigned int i = 0; i < (this->item.size()); i++)
	{
		if (! this->item[i])
			continue;

		if (this->item[i]->id == id)
		{
			// Either the type got messed up or we have
			// two items with the same id.
			if (this->item[i]->type != MenuItem::CHECKBOX)
				return false;

			// This cast may be dangerous if the type was
			// somehow changed.
			MenuItemCheckbox* c = (MenuItemCheckbox*)this->item[i];
			return c->isChecked();
		}
	}
	return false;
}
int Menu::getInt(int id)
{
	for (unsigned int i = 0; i < (this->item.size()); i++)
	{
		if (! this->item[i])
			continue;

		if (this->item[i]->id == id)
		{
			// Either the type got messed up or we have
			// two items with the same id.
			if (this->item[i]->type != MenuItem::NUMBERBOX)
				return -1;

			// This cast may be dangerous if the type was
			// somehow changed.
			MenuItemNumberbox* c = (MenuItemNumberbox*)this->item[i];
			return c->current;
		}
	}
	return -1;
}
std::string Menu::getString(int id)
{
	for (unsigned int i = 0; i < (this->item.size()); i++)
	{
		if (! this->item[i])
			continue;

		if (this->item[i]->id == id)
		{
			// Either the type got messed up or we have
			// two items with the same id.
			if (this->item[i]->type == MenuItem::TEXTBOX)
			{
				MenuItemTextbox* c = (MenuItemTextbox*)this->item[i];
				return c->currentText;
			}
			else if (this->item[i]->type == MenuItem::TEXTLIST)
			{
				MenuItemTextlist* c = (MenuItemTextlist*)this->item[i];
				return c->currentText();
			}
			else
				return "";

			// This cast may be dangerous if the type was
			// somehow changed.
		}
	}
	return "";
}
void Menu::reset()
{
	this->selected = false;
	this->selectedItem = NULL;
}


