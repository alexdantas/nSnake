#include "Board.hpp"
#include "Shapes.hpp"

Board::Board(int width, int height, bool haveBorders)
{
    this->clear(width, height);
    this->loadDefaultLevel();
    this->setBorders(haveBorders);
}
Board::~Board()
{ }
void Board::clear(int width, int height)
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

    // All the tiles are initialized by default with Tile::EMPTY
}
void Board::loadDefaultLevel()
{
    // The default level is simply a map with this->width and
    // this->height, boxed in with borders.
    for (int i = 0; i < (this->width); i++)
    {
        for (int j = 0; j < (this->height); j++)
        {
            // All the borders at the extremes of the board
            if ((i == 0) || (i == (width  - 1)) ||
                (j == 0) || (j == (height - 1)))
            {
                this->board[i][j].set(Tile::BORDER);
            }
        }
    }
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

    this->setBorders(this->borders);
    return true;
}
Level* Board::getLevel()
{
    return &(this->level);
}
bool Board::isInsideMap(int x, int y)
{
    Tile* tile = &(this->board[x][y]);

    if (tile->has(Tile::BORDER) ||
        tile->has(Tile::TELEPORT_BORDER))
        return true;

    // I will walk in all directions.
    // If I don't find a border in any of them, it's outside
    // the game map.
    //
    // (I know there are strange exceptions to this, but
    //  I can't find a simple way to do this.)
    //

    /// TODO A general pathfinding algorighm (A*?)

    Point origin(x, y);
    Point current = origin;

    // walking right
    while (!(this->board[current.x][current.y].isBorder()))
    {
        current.x++;
        if (current.x >= this->width)
            return false;
    }
    current = origin;

    // walking left
    while (!(this->board[current.x][current.y].isBorder()))
    {
        current.x--;
        if (current.x < 0)
            return false;
    }
    current = origin;

    // walking up
    while (!(this->board[current.x][current.y].isBorder()))
    {
        current.y--;
        if (current.y < 0)
            return false;
    }
    current = origin;

    // walking down
    while (!(this->board[current.x][current.y].isBorder()))
    {
        current.y++;
        if (current.y >= this->height)
            return false;
    }

    // Whew! If it got here, it means that exists borders
    // in ALL 4 sides of this point!
    //
    // Basically, it's in!
    return true;
}

