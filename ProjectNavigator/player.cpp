#include <SDL.h>

#include "tile.h"
#include "player.h"

Player::Player()
{
	moveUp = false;
	moveDown = false;
	moveLeft = false;
	moveRight = false;
}

Player::~Player()
{

}