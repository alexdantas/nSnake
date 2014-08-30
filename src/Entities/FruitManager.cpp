#include <Entities/FruitManager.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Config/Globals.hpp>

FruitManager::FruitManager(int amount):
	amount(amount)
{ }
bool FruitManager::eatenFruit(Player* player)
{
	// If any fruit was eaten by #player, we'll
	// delete it.
	for (std::vector<Fruit>::iterator it = this->fruit.begin(); it != this->fruit.end();)
	{
		if (player->headHit((*it).x, (*it).y))
		{
			// Alright, eaten!
			it = this->fruit.erase(it);
			return true;
		}
		else
			++it;
	}
	return false;
}
void FruitManager::update(Player* player, Board* board)
{
	// Creating enough fruits to fill the #amount quota.
	int diff = (this->amount - this->fruit.size());

	if (diff > 0)
		for (int i = 0; i < (diff); i++)
			this->addRandomly(board, player);
}
int FruitManager::getAmount()
{
	return (this->amount);
}
void FruitManager::add(int x, int y)
{
	this->fruit.push_back(Fruit(x, y));
}
void FruitManager::addRandomly(Board* board, Player* player)
{
	int newx = 1;
	int newy = 1;

	// Creating between the board limits,
	// making sure it isn't inside player's body.
	do
	{
		newx = Utils::Random::between(1, board->getW() - 2);
		newy = Utils::Random::between(1, board->getH() - 2);

	} while (player->bodyHit(newx, newy) ||
	         board->isWall(newx, newy));

	this->add(newx, newy);
}
void FruitManager::draw(Window* win)
{
	for (unsigned int i = 0; i < (this->fruit.size()); i++)
		win->print("$",
		           this->fruit[i].x,
		           this->fruit[i].y,
		           Globals::Theme::fruit);
}

