#ifndef BOARD_H_DEFINED
#define BOARD_H_DEFINED

#include <vector>
#include "Tile.hpp"

/// The game board, where all the action happens.
///
class Board
{
public:
    Board(int width, int height);
    virtual ~Board();

    /// Returns the pointer to the tile at index *x* *y*.
    Tile* at(int x, int y);

    void render(int x, int y);
    int getWidth();
    int getHeight();

    // TODO find out how to overload operators
    //      to make *board* access intuitive
    //      (just like at())

private:

    /// Matrix containing all the tiles of the game.
    std::vector<std::vector<Tile*> > board;

    int width;
    int height;
};

#endif //BOARD_H_DEFINED




