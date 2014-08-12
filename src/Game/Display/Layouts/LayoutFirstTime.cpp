#include <Game/Display/Layouts/LayoutFirstTime.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Game/Config/Globals.hpp>

LayoutFirstTime::LayoutFirstTime(int width, int height):
	Layout(width, height)
{
	this->windowsInit();
}
LayoutFirstTime::~LayoutFirstTime()
{
	this->windowsExit();
}
void LayoutFirstTime::windowsInit()
{
	Layout::windowsInit();
}
void LayoutFirstTime::windowsExit()
{
	clear();

	Layout::windowsExit();
}
void LayoutFirstTime::draw()
{

}

