#include "Board.hpp"

Board::Board(int width, int height)
{
    this->clear(width, height);
}
Board::~Board()
{ }
void Board::clear(int width, int height, bool haveBorders)
{
    // Clearing board if it had any elements.
    for (unsigned int i = 0; i < (this->board.size()); i++)
        this->board[i].clear();

    this->board.clear();

    this->width  = width;
    this->height = height;

    // Setting up 1st dimension
    this->board.resize(this->width);

    // Setting up 2nd dimension
    for (unsigned int i = 0; i < (this->board.size()); i++)
        this->board[i].resize(this->height);

    // Initializing tiles and placing the borders.
    for (int i = 0; i < (this->width); i++)
    {
        for (int j = 0; j < (this->height); j++)
        {
            // All the borders of the board
            if ((i == 0) || (i == (width  - 1)) ||
                (j == 0) || (j == (height - 1)))
            {
                if (haveBorders)
                    this->board[i][j].set(Tile::BORDER);
                else
                    this->board[i][j].set(Tile::TELEPORT_BORDER);
            }
        }
    }

    // Not using setBorders() because it will iterate through
    // the whole board again.
    this->borders = haveBorders;
}

Tile& Board::at(int x, int y)
{
    if ((x < 0) || (x >= this->width) ||
        (y < 0) || (y >= this->height))
        throw "Board::at Access to invalid index.";

    return (this->board[x][y]);
}
void Board::render(int x, int y)
{
    for (int i = 0; i < (this->width); i++)
        for (int j = 0; j < (this->height); j++)
            this->board[i][j].render(x + i, y + j);
}
int Board::getWidth()
{
    return (this->width);
}
int Board::getHeight()
{
    return (this->height);
}
void Board::setBorders(bool option)
{
    this->borders = option;

    // Refreshing the borders.
    for (int i = 0; i < (this->width); i++)
    {
        for (int j = 0; j < (this->height); j++)
        {
            if (this->board[i][j].has(Tile::BORDER) ||
                this->board[i][j].has(Tile::TELEPORT_BORDER))
            {
                if (this->borders)
                    this->board[i][j].set(Tile::BORDER);
                else
                    this->board[i][j].set(Tile::TELEPORT_BORDER);
            }
        }
    }
}
bool Board::hasBorders()
{
    return (this->borders);
}
int Board::maxLengthInsideMe()
{
    // We have a 1x1 border
    return (this->width - 1) * (this->height - 1);
}
bool Board::loadFile(std::string filename)
{
    bool result = this->level.load(filename);
    if (!result)
        return false;

    this->clear(this->level.getWidth(), this->level.getHeight());

    // now we parse the enum thingy into the actual board, nigga
    for (int i = 0; i < (this->width); i++)
        for (int j = 0; j < (this->height); j++)
            this->board[i][j].set(this->level.at(i, j));

    return true;
}
Level* Board::getLevel()
{
    return &(this->level);
}

