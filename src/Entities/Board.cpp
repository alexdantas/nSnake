#include <Entities/Board.hpp>
#include <Entities/Player.hpp>
#include <Engine/EngineGlobals.hpp>
#include <Engine/Helpers/Utils.hpp>

int Board::small_width  = 40;
int Board::small_height = 10;

int Board::medium_width  = 55;
int Board::medium_height = 14;

int Board::large_width  = 78;
int Board::large_height = 21;

Board::Board(int width, int height, Style style):
	style(style),
	start_x(BOARD_DEFAULT_PLAYER_X),
	start_y(BOARD_DEFAULT_PLAYER_Y)
{
	this->board = new Array2D<bool>(width, height);

	this->clear();
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
	int solid_appearance = ((EngineGlobals::Screen::fancy_borders) ?
	                        ACS_CKBOARD :
	                        '#');

	for (size_t i = 0; i < (this->board->width()); i++)
	{
		for (size_t j = 0; j < (this->board->height()); j++)
		{
			if (this->isBorder(i, j))
			{
				win->printChar(((this->style == Board::TELEPORT) ?
				                teleport_appearance :
				                solid_appearance),
				               i, j);
				continue;
			}
			else if (this->isWall(i, j))
				win->printChar(solid_appearance,
				               i, j);
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
void Board::clear()
{
	// Making it empty
	for (size_t i = 0; i < this->board->width(); i++)
		for (size_t j = 0; j < this->board->height(); j++)
			this->board->set(i, j, false);
}
void Board::setBoard(std::vector<std::vector<bool> >& newBoard)
{
	// Making it empty
	for (size_t i = 0; i < this->board->width(); i++)
		for (size_t j = 0; j < this->board->height(); j++)
			this->board->set(i, j, newBoard[j][i]);
}

int Board::getStartX()
{
	return this->start_x;
}
int Board::getStartY()
{
	return this->start_y;
}

void Board::setStartX(int x)
{
	this->start_x = x;
}
void Board::setStartY(int y)
{
	this->start_y = y;
}

void Board::setMetadata(std::string name, std::string value)
{
	this->metadata[name] = value;
}
std::string Board::getMetadata(std::string name)
{
	if (! this->hasMetadata(name))
		return "";

	return this->metadata[name];
}
bool Board::hasMetadata(std::string name)
{
	return (this->metadata.find(name) != this->metadata.end());
}
void Board::scrollLeft()
{
	// Going line by line from top to bottom
	for (size_t j = 0; j < this->board->height() - 1; j++)
	{
		// Get first left element from this line
		bool tmp = this->board->at(1, j);

		// Shifting all elements one block left
		for (size_t i = 0; i < (this->board->width() - 1); i++)
			this->board->set(i, j, this->board->at(i + 1, j));

		// Putting the first element on the last place
		this->board->set(this->board->width() - 2, j, tmp);
	}
}
void Board::scrollRight()
{
	// Going line by line from top to bottom
	for (size_t j = 0; j < this->board->height() - 1; j++)
	{
		// Get first right element from this line
		bool tmp = this->board->at(this->board->width() - 2, j);

		// Shifting all elements one block right
		for (size_t i = (this->board->width() - 1); i > 0; i--)
			this->board->set(i, j, this->board->at(i - 1, j));

		// Putting the first element on the last place
		this->board->set(1, j, tmp);
	}
}
void Board::scrollUp()
{
	// Going line by line from left to right
	for (size_t j = 0; j < this->board->width() - 1; j++)
	{
		// Get first top element from this line
		bool tmp = this->board->at(j, 1);

		// Shifting all elements one block up
		for (size_t i = 0; i < (this->board->height() - 1); i++)
			this->board->set(j, i, this->board->at(j, i + 1));

		// Putting the first element on the last place
		this->board->set(j, this->board->height() - 2, tmp);
	}
}
void Board::scrollDown()
{
	// Going line by line from left to right
	for (size_t j = 0; j < this->board->width() - 1; j++)
	{
		// Get first bottom element from this line
		bool tmp = this->board->at(j, this->board->height() - 2);

		// Shifting all elements one block down
		for (size_t i = this->board->height() - 2; i > 0; i--)
			this->board->set(j, i, this->board->at(j, i - 1));

		// Putting the first element on the last place
		this->board->set(j, 1, tmp);
	}
}

