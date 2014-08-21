#include <Display/Layouts/LayoutMainMenu.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Engine/Graphics/Ncurses.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <States/GameStateMainMenu.hpp>
#include <Display/Animations/AnimationSnakes.hpp>
#include <Display/Animations/AnimationFire.hpp>
#include <Display/Animations/AnimationWater.hpp>
#include <Display/Animations/AnimationGameOfLife.hpp>

#include <iostream>

LayoutMainMenu::LayoutMainMenu(int width, int height, GameStateMainMenu* state):
	Layout(width, height),
	state(state),
	logo(NULL),
	menu(NULL),
	animationwin(NULL),
	animation(NULL)
{
	this->windowsInit();
}
LayoutMainMenu::~LayoutMainMenu()
{
	this->windowsExit();
}
void LayoutMainMenu::windowsInit()
{
	Layout::windowsInit();

	// LOGO
	this->logo = new Window(this->main,
	                        0, 0,
	                        56, 7);
	this->logo->borders(Window::BORDER_NONE);

	// MENU
	this->menu = new Window(this->main,
	                        55, 0,
	                        24, WINDOW_FILL);

	this->menu->setTitle("Main Menu");
	this->menu->refresh();

	// ANIMATION
	this->animationwin = new Window(this->main,
	                                0,
	                                this->logo->getH(),
	                                this->logo->getW(),
	                                this->main->getH() - this->logo->getH() - 1);

	this->animationwin->borders(Window::BORDER_NONE);

	// Deciding randomly the type of the Animation
	switch(Utils::Random::between(0, 3))
	{
	case 0:
		this->animation = new AnimationWater(this->animationwin);
		break;

	case 1:
		this->animation = new AnimationSnakes(this->animationwin);
		break;

	case 2:
		this->animation = new AnimationGameOfLife(this->animationwin);
		break;

	default:
		this->animation = new AnimationFire(this->animationwin);
		break;
	}
	this->animation->load();
}
void LayoutMainMenu::windowsExit()
{
	SAFE_DELETE(this->menu);
	SAFE_DELETE(this->logo);
	SAFE_DELETE(this->animationwin);
	SAFE_DELETE(this->animation);

	Layout::windowsExit();
}
void LayoutMainMenu::draw(Menu* menu)
{
	this->animation->update();

	this->main->clear();

	this->animation->draw();

	this->logo->clear();
	this->logo->print(Utils::String::split(" __    _  _______  __    _  _______  ___   _  _______\n"
	                                       "|  |  | ||       ||  |  | ||   _   ||   | | ||       |\n"
	                                       "|   |_| ||  _____||   |_| ||  |_|  ||   |_| ||    ___|\n"
	                                       "|       || |_____ |       ||       ||      _||   |___ \n"
	                                       "|  _    ||_____  ||  _    ||       ||     |_ |    ___|\n"
	                                       "| | |   | _____| || | |   ||   _   ||    _  ||   |___ \n"
	                                       "|_|  |__||_______||_|  |__||__| |__||___| |_||_______|", '\n'),
	                  0,
	                  0,
	                  Colors::pair("green", "default", true));

	this->logo->refresh();

	// Yay!
	this->menu->clear();

	menu->draw(this->menu);

	this->menu->refresh();

	this->main->refresh();

	// NCURSES NEEDS THIS
	refresh();
}

