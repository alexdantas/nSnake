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
    Tile& at(int x, int y);

    /// Shows the whole board starting on *x* *y*, from top-left.
    void render(int x, int y);

    int getWidth();  ///< Returns the board's width.
    int getHeight(); ///< Returns the board's height.

    /// Tells if the player dies or teleports when colliding
    /// with the border of this Board.
    bool hasBorders();

    // TODO find out how to overload operators
    //      to make *board* access intuitive
    //      (just like at())

    /// Returns the max length any snake may have inside this board.
    ///
    /// Obviously it's the "usable" size inside the board.
    int maxLengthInsideMe();

private:

    /// Matrix containing all the tiles of the game.
    std::vector<std::vector<Tile> > board;

    int width;  ///< Width
    int height; ///< Height

    /// Tells if the borders are solid or teleportable.
    ///
    /// @note Does this word even exists?
    bool borders;
};

#endif //BOARD_H_DEFINED




