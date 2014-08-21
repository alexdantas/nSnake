#include <Display/Animations/AnimationFire.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Engine/Helpers/Utils.hpp>

static ColorPair red        = Colors::pair("red", "default");
static ColorPair redBold    = Colors::pair("red", "default", true);
static ColorPair white;
static ColorPair yellow     = Colors::pair("yellow", "default");
static ColorPair yellowBold = Colors::pair("yellow", "default", true);

// An ASCII gray scale :)
// Characters will be accessed proportionally to the intensity
// int gray_scale_size = 70;
// char gray_scale[71] = ".\'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$ ";

static int gray_scale_size = 12;
static char gray_scale[13] = " .':-=+*#%@#";

AnimationFire::AnimationFire(Window* window):
	Animation(window),
	particle(NULL)
{ }
AnimationFire::~AnimationFire()
{
	SAFE_DELETE(particle);
}
void AnimationFire::load()
{
	unsigned int width  = window->getW();
	unsigned int height = window->getH();

	particle = new Array2D<ParticleFire>(width, height);

	// Creating the cooling map
	coolingMap = new Array2D<int>(width, height);

	for (unsigned int i = 0; i < width; i++)
		for (unsigned int j = 0; j < height; j++)
			coolingMap->set(i, j, Utils::Random::between(INTENSITY_MIN,
			                                             INTENSITY_PERCENT(13)));

	// Will smooth the cooling map a number of times
	for (int n = 0; n < 10; n++)
		for (unsigned int i = 1; i < width-1; i++)
			for (unsigned int j = 1; j < height-1; j++)
				coolingMap->set(i, j, (coolingMap->at(i-1, j) +
				                       coolingMap->at(i+1, j) +
				                       coolingMap->at(i, j+1) +
				                       coolingMap->at(i, j-1)) / 4);

	timer.start();
}
void AnimationFire::update()
{
	// Updating only at the right time!
	if (timer.delta_ms() < 100)
		return;

	// How fast the fire cools down each frame
	int cooling_ratio = Utils::Random::between(INTENSITY_PERCENT(3),
	                                           INTENSITY_PERCENT(12));

	// Slim chance of a sudden burst or dim of the fire
	bool burst = Utils::Random::booleanWithChance(0.10);
	bool dim   = Utils::Random::booleanWithChance(0.12);
	if (burst) cooling_ratio = INTENSITY_PERCENT(1);
	if (dim)   cooling_ratio = INTENSITY_PERCENT(30);

	// Spawning high-intensity flames on the bottom particles
	for (unsigned int i = 0; i < (particle->width()); i++)
		particle->set(i, particle->height() - 1, ParticleFire(Utils::Random::between(INTENSITY_PERCENT(90), INTENSITY_MAX)));

	// Randomly adding Sparks - high-intensity flames little higher
	for (unsigned int i = 0; i < (particle->width()); i++)
	{
		if (Utils::Random::booleanWithChance(2.31))
		{
			int height = particle->height() - Utils::Random::between(3, 6);

			particle->set(i, height, ParticleFire(Utils::Random::between(INTENSITY_PERCENT(90), INTENSITY_MAX)));
		}
	}

	// Making all particles climb up
	for (unsigned int i = 0; i < (particle->width()); i++)
	{
		for (unsigned int j = 0; j < (particle->height()-1); j++)
		{
			// Cooling all particles based on the ones below
			particle->set(i, j, ParticleFire(particle->at(i, j + 1).intensity - cooling_ratio));

			// Cooling based on the cooling map
			particle->set(i, j, ParticleFire(particle->at(i, j).intensity - coolingMap->at(i, j)));
		}
	}

	timer.start();
}
void AnimationFire::draw()
{
	for (unsigned int i = 0; i < (particle->width()); i++)
	{
		for (unsigned int j = 0; j < (particle->height()); j++)
		{
			int       c = ' ';
			ColorPair p = white;
			int       s = particle->at(i, j).intensity;

			if (s > INTENSITY_PERCENT(90))
				p = white;

			else if (s > INTENSITY_PERCENT(80))
				p = yellowBold;

			else if (s > INTENSITY_PERCENT(70))
				p = yellowBold;

			else if (s > INTENSITY_PERCENT(60))
				p = yellow;

			else if (s > INTENSITY_PERCENT(50))
				p = redBold;

			else if (s > INTENSITY_PERCENT(40))
				p = redBold;

			else if (s > INTENSITY_PERCENT(30))
				p = red;

			else if (s > INTENSITY_PERCENT(20))
				p = red;

			else
				continue; // Too low intensity


			if ((s > INTENSITY_MAX) || (s < INTENSITY_MIN))
				continue;

			else
				c = gray_scale[(s - INTENSITY_MIN) * (gray_scale_size-1)/INTENSITY_MAX];

			window->printChar(c, i, j, p);
		}
	}
}

