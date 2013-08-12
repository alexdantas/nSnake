#ifndef BOARD_H_DEFINED
#define BOARD_H_DEFINED

#include <vector>
#include "Tile.hpp"

/// The game board, where all the action happens.
///
class Board
{
public:
    Board(int width, int height, bool hasBorders);
    virtual ~Board();

    /// Returns the pointer to the tile at index *x* *y*.
    Tile* at(int x, int y);

    void render(int x, int y);
    int getWidth();
    int getHeight();

    /// Tells if the player dies or teleports when colliding
    /// with the border of this Board.
    bool hasBorders();

    // TODO find out how to overload operators
    //      to make *board* access intuitive
    //      (just like at())

private:

    /// Matrix containing all the tiles of the game.
    std::vector<std::vector<Tile*> > board;

    int width;
    int height;

    /// Tells if the borders are solid or teleportable.
    ///
    /// @note Does this word even exists?
    bool borders;
};

#endif //BOARD_H_DEFINED




