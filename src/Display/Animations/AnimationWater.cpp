#include <Display/Animations/AnimationWater.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Engine/Helpers/Utils.hpp>

static ColorPair white;
static ColorPair blue     = Colors::pair("blue", "default"      );
static ColorPair blueBold = Colors::pair("blue", "default", true);
static ColorPair cyan     = Colors::pair("cyan", "default"      );
static ColorPair cyanBold = Colors::pair("cyan", "default", true);

static int gray_scale_size = 11;
static char gray_scale[12] = "#@%#*+=-:'.";

AnimationWater::AnimationWater(Window* window):
	Animation(window),
	buffer1(NULL),
	buffer2(NULL)
{ }
AnimationWater::~AnimationWater()
{
	SAFE_DELETE(buffer1);
	SAFE_DELETE(buffer2);
}
void AnimationWater::load()
{
	unsigned int width  = window->getW();
	unsigned int height = window->getH();

	buffer1 = new Array2D<int>(width, height);
	buffer2 = new Array2D<int>(width, height);

	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			buffer1->set(i, j, Utils::Random::between(HEIGHT_MIN,
			                                          HEIGHT_PERCENT(13)));
			buffer2->set(i, j, Utils::Random::between(HEIGHT_MIN,
			                                          HEIGHT_PERCENT(25)));
		}
	}

	timer.start();
}
void AnimationWater::update()
{
	// Updating only at the right time!
	if (timer.delta_ms() < 300)
		return;

	// Swapping the buffers
	Array2D<int>* tmp = buffer1;
	buffer1 = buffer2;
	buffer2 = tmp;

	// Randomly adding a light point
	if (Utils::Random::booleanWithChance(0.31))
		buffer2->set(Utils::Random::between(0, buffer2->width()-1),
		             Utils::Random::between(0, buffer2->height()-1), HEIGHT_PERCENT(90));

	// Dont update the edges
	for (unsigned int i = 1; i < (buffer1->width() - 1); i++)
	{
		for (unsigned int j = 1; j < (buffer1->height() - 1); j++)
		{
			// Instead of dividing by two,
			// we're shifting by one
			buffer2->set(i, j, ((buffer1->at(i-1, j) +
			                     buffer1->at(i+1, j) +
			                     buffer1->at(i, j+1) +
			                     buffer1->at(i, j-1)) >> 1) - buffer2->at(i, j));

//			buffer2->at(i, j) -= (buffer2->at(i, j) >> 5);
		}
	}

	timer.start();
}
void AnimationWater::draw()
{
	for (unsigned int i = 0; i < (buffer2->width()); i++)
	{
		for (unsigned int j = 0; j < (buffer2->height()); j++)
		{
			int       c = ' ';
			ColorPair p = white;
			int       s = buffer2->at(i, j);

			if (s > HEIGHT_PERCENT(80))
				p = white;

			else if (s > HEIGHT_PERCENT(60))
				p = cyanBold;

			else if (s > HEIGHT_PERCENT(40))
				p = cyan;

			else if (s > HEIGHT_PERCENT(20))
				p = blue;

			else
				p = blueBold;

			if ((s > HEIGHT_MAX) || (s < HEIGHT_MIN))
				continue;

			else
				c = gray_scale[(s - HEIGHT_MIN) * (gray_scale_size-1)/HEIGHT_MAX];

			window->printChar(c, i, j, p);
		}
	}
}

