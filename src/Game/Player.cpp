#include <Game/Player.hpp>

Player::Player(int x, int y):
	alive(true),
	currentDirection(Direction::RIGHT),
	nextDirection(Direction::RIGHT)
{
	// Initializing the snake with 3 bodies
	for (int i = 0; i < 3; i++)
		this->body.push_back(Body(x - i, y));
}
bool Player::isAlive()
{
	return (this->alive);
}
int Player::getSize()
{
	return (int)(this->body.size());
}
int Player::getX()
{
	return (this->body.front().x);
}
int Player::getY()
{
	return (this->body.front().y);
}
void Player::move(Direction dir)
{
	this->nextDirection = dir;
}
void Player::kill()
{
	this->alive = false;
}
void Player::update()
{
	// We have to make sure the snake doesn't do strange
	// things, like turning around on itself.
	switch(this->nextDirection)
	{
	case Direction::RIGHT:
		if (this->currentDirection != Direction::LEFT)
			this->currentDirection = this->nextDirection;
		break;

	case Direction::LEFT:
		if (this->currentDirection != Direction::RIGHT)
			this->currentDirection = this->nextDirection;
		break;

	case Direction::UP:
		if (this->currentDirection != Direction::DOWN)
			this->currentDirection = this->nextDirection;
		break;

	case Direction::DOWN:
		if (this->currentDirection != Direction::UP)
			this->currentDirection = this->nextDirection;
		break;
	};

	// Making the rest of the body catch up
	for (unsigned int i = (this->body.size() - 1); i > 0; i--)
	{
		this->body[i].x = this->body[i - 1].x;
		this->body[i].y = this->body[i - 1].y;
	}

	// Moving the head
	switch(this->currentDirection)
	{
	case Direction::RIGHT:
		this->body.front().x++;
		break;

	case Direction::LEFT:
		this->body.front().x--;
		break;
	case Direction::UP:
		this->body.front().y--;
		break;

	case Direction::DOWN:
		this->body.front().y++;
		break;
	}
}
void Player::draw(Window* win)
{
	// The head
	win->printChar(((this->alive) ?
	                '@' :
	                'X'),
	               this->body.front().x,
	               this->body.front().y,
	               Colors::pair(COLOR_GREEN, COLOR_DEFAULT, true));

	// Rest of the body
	for (unsigned int i = 1; i < (this->body.size()); i++)
		win->printChar('o',
		               this->body[i].x,
		               this->body[i].y,
		               Colors::pair(COLOR_GREEN, COLOR_DEFAULT, true));
}
bool Player::collideWithItself()
{
	for (unsigned int i = 3; i < (this->body.size()); i++)
		if ((this->body[i].x == this->body.front().x) &&
		    (this->body[i].y == this->body.front().y))
			return true;

	return false;
}

