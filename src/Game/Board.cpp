#include <Game/Board.hpp>
#include <Game/Player.hpp>
#include <Config/Globals.hpp>
#include <Misc/Utils.hpp>

Board::Board(int width, int height, Style style):
	style(style)
{
	this->board = new Array2D<bool>(width, height);

	// Making it empty
	for (size_t i = 0; i < this->board->width(); i++)
		for (size_t j = 0; j < this->board->height(); j++)
			this->board->set(i, j, false);
}
Board::~Board()
{
	delete this->board;
}
bool Board::isBorder(int x, int y)
{
	if ((x == 0) || (x == (int)this->board->width()  - 1) ||
	    (y == 0) || (y == (int)this->board->height() - 1))
		return true;

	return false;
}
bool Board::isWall(int x, int y)
{
	if (isBorder(x, y))
	{
		// If we can teleport, the borders are
		// not collidable - we'll just walk through them.
		return (this->style == Board::SOLID);
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
	int teleport_appearance = '\'';
	int solid_appearance = ((Globals::Screen::fancy_borders) ?
	                        ACS_CKBOARD :
	                        '#');

	for (size_t i = 0; i < (this->board->width()); i++)
	{
		for (size_t j = 0; j < (this->board->height()); j++)
		{
			// Drawing the borders
			if ((i == 0) || (i == this->board->width() - 1) ||
			    (j == 0) || (j == this->board->height() - 1))
			{
				win->printChar(((this->style == Board::TELEPORT) ?
				                teleport_appearance :
				                solid_appearance),
				               i,
				               j,
				               0);
				continue;
			}

		    // Drawing the walls
			if (this->board->at(i, j))
				win->printChar(solid_appearance,
				               i,
				               j,
				               0);
		}
	}
}
void Board::randomlyFillExceptBy(int x, int y)
{
	for (size_t i = 0; i < (this->board->width()); i++) {
		for (size_t j = 0; j < (this->board->height()); j++) {

			// The frequency of random Walls (cute PI)
			if (Utils::Random::booleanWithChance(0.031415923))
				this->board->set(i, j, true);
		}
	}

	// Clearing some space for #x and #y
	for (int i = -2; i != 7; i++)
		this->board->set(x + i, y, false);
}
void Board::teleport(Player* player)
{
	// If we don't teleport,
	// at least keep it on the current position
	int newx = player->getX();
	int newy = player->getY();

	// Where we'll place the player
	int left  = 1;
	int right = this->board->width() - 2;

	if (player->getX() < left)
	{
		newx = right;
	}
	else if (player->getX() > right)
	{
		newx = left;
	}

	int top    = 1;
	int bottom = this->board->height() - 2;

	if (player->getY() < top)
	{
		newy = bottom;
	}
	else if (player->getY() > bottom)
	{
		newy = top;
	}

	player->moveTo(newx, newy);
}

