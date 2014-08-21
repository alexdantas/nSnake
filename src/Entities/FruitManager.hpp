#ifndef FRUITMANAGER_H_DEFINED
#define FRUITMANAGER_H_DEFINED

#include <Entities/Player.hpp>
#include <Entities/Board.hpp>
#include <Engine/Graphics/Window.hpp>

#include <vector>

/// A single fruit.
struct Fruit
{
	int x;
	int y;
	Fruit(int x, int y):
		x(x),
		y(y)
	{ }
};

/// Controls how many Fruits are there and how they're spawned.
class FruitManager
{
public:
	/// Creates a Fruit container that has at most
	/// #amount fruits at once on the screen.
	FruitManager(int amount);

	virtual ~FruitManager() {};

	/// Tells if the #player has eaten a fruit this frame.
	bool eatenFruit(Player* player);

	/// Updates internal fruits, adding them to the #board
	/// and making sure it doesn't touch #player.
	void update(Player* player, Board* board);

	/// Returns the maximum size we can store within
	/// this manager.
	///
	/// @note This is not the current size, as you can
	///       use #add and #addRandomly to forcefully
	///       add beyond it's limit.
	int getAmount();

	/// Creates a fruit, adding it at #x, #y.
	/// @note It ignores internal #amount.
	void add(int x, int y);

	/// Creates a fruit randomly within boundaries of #board,
	/// making sure that it's not inside #player.
	///
	/// @note It ignores internal #amount.
	void addRandomly(Board* board, Player* player);

	void draw(Window* win);

private:
	std::vector<Fruit> fruit;
	int amount;
};

#endif //FRUITMANAGER_H_DEFINED

