#include <Display/Animations/AnimationSnakes.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Engine/Helpers/Utils.hpp>

AnimationSnakes::AnimationSnakes(Window* window):
	Animation(window)
{ }
void AnimationSnakes::load()
{
	this->addSnake();

	updateTimer.start();
	addTimer.start();
}
void AnimationSnakes::update()
{
	// Adding yet another snake
	int delay = Utils::Random::between(1, 3) * 100;

	if ((addTimer.delta_ms() > delay) &&
	    (this->lilsnakes.size() < MAX_SNAKES))
	{
		this->addSnake();

		// Random add burst!
		if (Utils::Random::booleanWithChance(0.2501))
		{
			for (int i = 0; i < Utils::Random::between(3, 5); i++)
				this->addSnake();
		}

		addTimer.start();
	}

	// Updating all snakes
	// They all drop once and get deleted as soon as they
	// leave the Window.
	if (updateTimer.delta_ms() > 50)
	{
		std::vector<LilSnake>::iterator it = this->lilsnakes.begin();

		while (it != this->lilsnakes.end())
		{
			if (((*it).y - (*it).size) > (this->window->getH() - 1))
			{
				it = this->lilsnakes.erase(it);
			}
			else
			{
				(*it).y++;
				++it;
			}
		}
		updateTimer.start();
	}
}
void AnimationSnakes::draw()
{
	for (unsigned int i = 0; i < (this->lilsnakes.size()); i++)
	{
		window->printChar('@',
		                  this->lilsnakes[i].x,
		                  this->lilsnakes[i].y,
		                  Colors::pair("green", "default", true));

		for (int j = 1; j < (this->lilsnakes[i].size); j++)
		{
			window->printChar('o',
			                  this->lilsnakes[i].x,
			                  this->lilsnakes[i].y - j,
			                  Colors::pair("green", "default"));

		}
	}
}
void AnimationSnakes::addSnake()
{
	int newx = Utils::Random::between(1, this->window->getW() - 1);
	int newy = Utils::Random::between(0, 3);
	int size = Utils::Random::between(2, 14);

	this->lilsnakes.push_back(LilSnake(newx, newy, size));
}

