#ifndef BOARD_H_DEFINED
#define BOARD_H_DEFINED

#include <Engine/Helpers/Array2D.hpp>
#include <Engine/Graphics/Window.hpp>

#include <map>

// Avoiding circular #include hell.
class Player;

// Default starting point for
// the player on every level
#define BOARD_DEFAULT_PLAYER_X 2
#define BOARD_DEFAULT_PLAYER_Y 2

/// A level where the snake runs and eats fruits.
///
/// @note I couldn't name this class "Level" because it could
///       get confusing with the concept of game speed, which is
///       also based on the word "Level".
///
/// This class represents a single level the snake can play on.
///
/// It is essentially a 2D matrix of tiles, where each can be
/// either "Walls" or "Empty.
///
/// It also contains some extra info, like the position where
/// the player should spawn, and (if this level was created by
/// someone) the author, date and stuff.
///
class Board
{
public:
	static int small_width;
	static int small_height;

	static int medium_width;
	static int medium_height;

	static int large_width;
	static int large_height;

	/// If the player will teleport when reaching the
	/// Board's limits or not.
	enum Style { SOLID, TELEPORT };

	/// Creates a new Board.
	///
	/// @param width   Whole level width
	/// @param height  Whole level height
	/// @param style   If the player will teleport when reaching the limits
	///
	Board(int width, int height, Style style);

	virtual ~Board();

	/// Tells if there's a wall at #x #y.
	bool isWall(int x, int y);
	bool isBorder(int x, int y);

	int getW();
	int getH();

	void draw(Window* win);

	/// Places random walls all over the Board
	/// except by #x and #y, allowing the Player
	/// to move a little bit when starting.
	void randomlyFillExceptBy(int x, int y);

	/// Makes the `Player` teleport if it's on a border.
	void teleport(Player* player);

	/// Makes the whole level empty.
	void clear();

	/// Sets the whole level content.
	///
	/// @param newBoard 2D matrix of booleans, telling if
	///                 there's a "Wall" on that tile or not.
	///
	void setBoard(std::vector<std::vector<bool> >& newBoard);

	/// Tells if the player will teleport when
	/// reaching the Board's limits or not.
	///
	/// @note The Board adapts automatically if you
	///       change this on-the-fly.
	Style style;

	int getStartX();
	int getStartY();

	void setStartX(int x);
	void setStartY(int y);

	// Things related to metadata
	// (author name, date made, comments, and more)

	/// Sets a meta information from this level.
	void setMetadata(std::string name, std::string value);

	/// Gets a meta information from this level.
	///
	/// @return Requested info or empty string if not exists.
	///
	std::string getMetadata(std::string name);

	/// Tells if this level has a specific information attached.
	bool hasMetadata(std::string name);

	void scrollLeft();
	void scrollRight();
	void scrollUp();
	void scrollDown();

private:
	/// The actual level on the screen.
	/// `true` means there's a wall here.
	/// `false` means the player can walk through.
	Array2D<bool>* board;

	int start_x; ///< Where the player will start (x axis)
	int start_y; ///< Where the player will start (y axis)

	/// Contains all this level's metadata.
	std::map<std::string, std::string> metadata;
};

#endif //BOARD_H_DEFINED

