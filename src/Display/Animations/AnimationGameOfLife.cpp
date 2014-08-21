#include <Display/Animations/AnimationGameOfLife.hpp>
#include <Engine/Graphics/Colors.hpp>
#include <Engine/Helpers/Utils.hpp>

static ColorPair white;
static ColorPair yellow = Colors::pair("yellow", "default");

AnimationGameOfLife::AnimationGameOfLife(Window* window):
	Animation(window),
	cells(NULL)
{ }
AnimationGameOfLife::~AnimationGameOfLife()
{
	SAFE_DELETE(cells);
}
void AnimationGameOfLife::load()
{
	unsigned int width  = window->getW();
	unsigned int height = window->getH();

	cells = new Array2D<bool>(width, height);

	// TODO: define an initial pattern?
	for (unsigned int i = 0; i < width; i++)
		for (unsigned int j = 0; j < height; j++)
			cells->set(i, j, Utils::Random::booleanWithChance(0.20));

	timer.start();
}
void AnimationGameOfLife::update()
{
	// Updating only at the right time!
	if (timer.delta_ms() < 200)
		return;

	// Dont update the edges
	for (unsigned int i = 1; i < (cells->width() - 1); i++)
	{
		for (unsigned int j = 1; j < (cells->height() - 1); j++)
		{
			// The neighbors, biatch!
			// Suppose the cell is [x], the neighbors are numbered
			// from 0 to 7 like this:
			//
			//                      [0][1][2]
			//                      [3][X][4]
			//                      [5][6][7]
			//
			bool n[8];

			n[0] = cells->at(i - 1, j - 1);
			n[1] = cells->at(i    , j - 1);
			n[2] = cells->at(i + 1, j - 1);
			n[3] = cells->at(i - 1, j    );
			n[4] = cells->at(i + 1, j    );
			n[5] = cells->at(i - 1, j + 1);
			n[6] = cells->at(i    , j + 1);
			n[7] = cells->at(i + 1, j + 1);

			int alive = 0;
			for (int x = 0; x < 8; x++)
				if (n[x])
					alive++;

			if (cells->at(i, j))
			{
				// Cell is currently alive!
				if (alive < 2)
					cells->set(i, j, false);

				else if (alive < 4)
					cells->set(i, j, true);

				else
					cells->set(i, j, false);
			}
			else
			{
				// Cell is currently dead!
				if (alive == 3)
					cells->set(i, j, true);
			}
		}
	}

	timer.start();
}
void AnimationGameOfLife::draw()
{
	for (unsigned int i = 0; i < (cells->width()); i++)
	{
		for (unsigned int j = 0; j < (cells->height()); j++)
		{
			int       c = ' ';
			ColorPair p = white;

			if (cells->at(i, j))
			{
				c = '#';
				p = yellow;
			}
			window->printChar(c, i, j, p);
		}
	}
}

