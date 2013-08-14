#ifndef FOODMANAGER_H_DEFINED
#define FOODMANAGER_H_DEFINED

#include <list>
#include "Board.hpp"

/// Responsible for inserting and updating foods on a board.
class FoodManager
{
public:
    /// Creates a new FoodManager on *board*.
    FoodManager(Board* board);

    virtual ~FoodManager();

    /// Updates all foods.
    void update();

    /// Adds a food on *x* *y*.
    void add(int x, int y);

    /// Adds a food randomly on the board.
    void addAtRandom();

private:
    /// The board that the FoodManager interacts with.
    Board* board;

    /// References to the tiles that contain the foods on the board.
    std::list<Tile*> foods;
};

#endif //FOODMANAGER_H_DEFINED

