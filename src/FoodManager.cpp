#include <algorithm>
#include "FoodManager.hpp"
#include "Ncurses.hpp"

FoodManager::FoodManager(Board* board):
    board(board)
{ }
FoodManager::~FoodManager()
{ }
void FoodManager::update()
{
    for (std::list<Tile*>::iterator it = this->foods.begin();
         it != this->foods.end();
         ++it)
    {
        Tile* tile = (*it);
        // Player has eaten the food
        // If you want to generate the fruit AFTER the player
        // eats it, uncomment this
        // if (!(tile->has(Tile::FOOD)))

        // We're currently generate another fruit AS the player
        // is eating the fruit
        if (tile->has(Tile::SNAKE_HEAD))
        {
            tile->remove(Tile::FOOD);

            // Remove the tile from the list
            // and add a new fruit

            // std::vector<Tile>::iterator it2 = it;
            // std::swap(it2, this->foods.end());
            // this->foods.pop_back();
            // it--;
            this->addAtRandom();

            it = this->foods.erase(it);
            if (it != this->foods.begin())
                it--;
        }
    }
}
void FoodManager::add(int x, int y)
{
    Tile* tile = &(this->board->at(x, y));

    tile->add(Tile::FOOD);
    this->foods.push_back(tile);
}
void FoodManager::addAtRandom()
{
    int x, y;
    int w = this->board->getWidth();
    int h = this->board->getHeight();

    // Will keep getting new x and y while the point formed
    // with them is _not empty_ OR _not inside the map_.
    do
    {
        x = Ncurses::randomNumberBetween(1, (w - 2)); // borders
        y = Ncurses::randomNumberBetween(1, (h - 2)); // borders

    } while (!(this->board->at(x, y).isEmpty()) ||
             !(this->board->isInsideMap(x, y)));

    this->add(x, y);
}


