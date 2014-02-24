#include <Interface/LayoutMainMenu.hpp>
#include <Interface/Colors.hpp>
#include <Interface/Ncurses.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>
#include <Flow/GameStateMainMenu.hpp>

#include <iostream>

LayoutMainMenu::LayoutMainMenu(int width, int height, GameStateMainMenu* state):
	Layout(width, height),
	state(state),
	logo(nullptr),
	menu(nullptr)
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
	                        0,
	                        0,
	                        56,
	                        9);

	// MENU
	this->menu = new Window(this->main,
	                        56,
	                        0,
	                        23,
	                        WINDOW_FILL);
	this->menu->setTitle("Main Menu");

	if (Globals::Screen::show_borders)
	{
		this->menu->borders(Globals::Screen::fancy_borders ?
		                    Window::BORDER_FANCY :
		                    Window::BORDER_REGULAR);
	}
	this->menu->refresh();
}
void LayoutMainMenu::windowsExit()
{
	SAFE_DELETE(this->menu);
	SAFE_DELETE(this->logo);

	Layout::windowsExit();
}
void LayoutMainMenu::draw(Menu* menu)
{
	this->main->clear();

	this->logo->clear();
	this->logo->print_multiline(" __    _  _______  __    _  _______  ___   _  _______\n"
	                            "|  |  | ||       ||  |  | ||   _   ||   | | ||       |\n"
	                            "|   |_| ||  _____||   |_| ||  |_|  ||   |_| ||    ___|\n"
	                            "|       || |_____ |       ||       ||      _||   |___ \n"
	                            "|  _    ||_____  ||  _    ||       ||     |_ |    ___|\n"
	                            "| | |   | _____| || | |   ||   _   ||    _  ||   |___ \n"
	                            "|_|  |__||_______||_|  |__||__| |__||___| |_||_______|",
	                            0,
	                            0,
	                            Colors::pair(COLOR_RED, COLOR_DEFAULT));

	this->logo->refresh();

	// Yay!
	this->menu->clear();

	menu->draw(this->menu);

	this->menu->refresh();

	this->main->refresh();

	// NCURSES NEEDS THIS
	refresh();
}

