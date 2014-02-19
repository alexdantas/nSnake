#include <Game/Board.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

Board::Board(int width, int height, Style style):
	style(style)
{
	this->board = new Array2D<bool>(width, height);

	// Making it empty
	for (size_t i = 0; i < this->board->width(); i++)
	{
		for (size_t j = 0; j < this->board->height(); j++)
		{
			if (Globals::Game::random_mode)
				this->board->set(i, j, Utils::Random::booleanWithChance(10));
			else
				this->board->set(i, j, false);
		}
	}
}
Board::~Board()
{
	delete this->board;
}
bool Board::isWall(int x, int y)
{
	// Special case - the borders
	if ((x == 0) || (x == (int)this->board->width() - 1) ||
	    (y == 0) || (y == (int)this->board->height() - 1))
	{
		// If we can teleport, the borders are
		// not collidable - we'll just walk through them.
		return (this->style == Style::SOLID);
	}

	return (this->board->at(x, y));
}
int Board::getW()
{
	return this->board->width();
}
int Board::getH()
{
	return this->board->height();
}
void Board::draw(Window* win)
{
	for (size_t i = 0; i < (this->board->width()); i++)
	{
		for (size_t j = 0; j < (this->board->height()); j++)
		{
			// Drawing the borders
			if ((i == 0) || (i == this->board->width() - 1) ||
			    (j == 0) || (j == this->board->height() - 1))
			{
				win->printChar(((this->style == Style::TELEPORT) ?
				                '.' :
				                '#'),
				               i,
				               j,
				               0);
				continue;
			}

		    // Drawing the walls
			if (this->board->at(i, j))
				win->printChar('#',
				               i,
				               j,
				               0);
		}
	}
}

