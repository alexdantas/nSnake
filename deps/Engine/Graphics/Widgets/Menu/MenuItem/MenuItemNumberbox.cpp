#include <Engine/Graphics/Widgets/Menu/MenuItem/MenuItemNumberbox.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/Helpers/Timer.hpp>
#include <Engine/InputManager.hpp>

MenuItemNumberbox::MenuItemNumberbox(std::string label, int id, int min, int max, int initial, int jump):
	MenuItem(label, id),
	min(min),
	max(max),
	initial(initial),
	current(initial),
	jump(jump)
{
	this->type = MenuItem::NUMBERBOX; // placing it above wont work
}
void MenuItemNumberbox::draw(Window* window, int x, int y, int width, bool hilite)
{
	std::string number = Utils::String::toString(this->current);

	// Will draw
	//      label     text
	// If not hilite.
	// If hilite:
	//      label   < text >
	MenuItem::draw(window,
	               x,
	               y,
	               (width - number.size() - 2),
	               hilite);

	int rightmost = x + width;

	window->print(((hilite)?
	               "<":
	               "["),
	              rightmost - number.size() - 2,
	              y,
	              ((hilite)?
	               EngineGlobals::Theme::hilite_text:
	               EngineGlobals::Theme::text));

	window->print(((hilite)?
	               ">":
	               "]"),
	              rightmost - 1,
	              y,
	              ((hilite)?
	               EngineGlobals::Theme::hilite_text:
	               EngineGlobals::Theme::text));

	window->print(number,
	              rightmost - number.size() - 1,
	              y,
	              EngineGlobals::Theme::hilite_text);
}
void MenuItemNumberbox::handleInput()
{
	if (InputManager::noKeyPressed())
		return;

	// These will allow the user to type numbers
	// and set the current value.
	// It the user press numbers within a well-defined
	// time delta, they'll add up to the current value
	// until hit the maximum.
	// If the user doesn't press a number for that
	// period, the timer "resets".
	static Timer lastKeyTimer;
	static int   lastKeyDelay = 500;
	static bool  firstDigit   = false;
	static bool  secondDigit  = false;
	static bool  thirdDigit   = false;

	int input = InputManager::pressedKey;

	// Special case, input was a number
	if (input >= '0' && input <= '9')
	{
		if (! firstDigit)
		{
			this->set(input - '0');
			firstDigit = true;
			lastKeyTimer.start();
			return;
		}

		if (lastKeyTimer.delta_ms() < lastKeyDelay)
		{
			if (! secondDigit)
			{
				this->set(this->current * 10 + (input - '0'));
				secondDigit = true;
				lastKeyTimer.start();
				return;
			}
			if (! thirdDigit)
			{
				this->set(this->current * 10 + (input - '0'));
				thirdDigit = true;
				lastKeyTimer.start();
				return;
			}
		}
		else
		{
			// reset everything
			this->set(input - '0');
			firstDigit  = true;
			secondDigit = false;
			thirdDigit  = false;
			lastKeyTimer.start();

		}
		return;
	}

	// Anything else
	if (InputManager::isPressed("left") || // user-defined
	    InputManager::isPressed(KEY_LEFT))
		this->decrease();

	else if (InputManager::isPressed("right") ||
	         InputManager::isPressed(KEY_RIGHT))
		this->increase();

	else if (InputManager::isPressed('r')  ||
	         InputManager::isPressed('R')  ||
	         InputManager::isPressed(' ')  ||
	         InputManager::isPressed('\n') ||
	         InputManager::isPressed(KEY_ENTER))
		this->reset();
}
void MenuItemNumberbox::set(int value)
{
	this->current = value;
	this->cap();
}
void MenuItemNumberbox::increase()
{
	this->current += this->jump;
	this->cap();
}
void MenuItemNumberbox::decrease()
{
	this->current -= this->jump;
	this->cap();
}
void MenuItemNumberbox::reset()
{
	this->current = this->initial;
}
void MenuItemNumberbox::cap()
{
	if (this->current > this->max)
		this->current = this->max;

	if (this->current < this->min)
		this->current = this->min;
}
