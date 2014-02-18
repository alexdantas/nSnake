#include "Player.hpp"

Player::Player(int x, int y):
	alive(true),
	currentDirection(Direction::RIGHT)
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
void Player::move(Direction dir)
{
	switch(dir)
	{
	case Direction::RIGHT:
		if (this->currentDirection == Direction::LEFT)
			return;
		break;

	case Direction::LEFT:
		if (this->currentDirection == Direction::RIGHT)
			return;
		break;

	case Direction::UP:
		if (this->currentDirection == Direction::DOWN)
			return;
		break;

	case Direction::DOWN:
		if (this->currentDirection == Direction::UP)
			return;
		break;
	};
	this->currentDirection = dir;
}
void Player::update()
{
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
	win->print("@",
	           this->body.front().x,
	           this->body.front().y,
	           Colors::pair(COLOR_GREEN, COLOR_DEFAULT, true));

	for (unsigned int i = 1; i < (this->body.size()); i++)
		win->print("o",
		           this->body[i].x,
		           this->body[i].y,
		           Colors::pair(COLOR_GREEN, COLOR_DEFAULT, true));
}

