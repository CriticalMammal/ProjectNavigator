#include <SDL.h>

#include "tile.h"

using namespace std;


Tile::Tile()
{
	collision = false;
	empty = false;
	type = none;
	tileTexture = NULL;

	x = 0;
	y = 1000;
	z = 1;
	goalX = x;
	goalY = y;
	goalZ = z;
	xLerp = 0;
	yLerp = 0;
	zLerp = 0;
	
	width = 5;
	height = 5;

	prevMovementTime = SDL_GetTicks();
	movementWaitTime = 0.2*1000; // In milliseconds

	isEditTile = false;
}


Tile::~Tile()
{
	SDL_DestroyTexture(tileTexture);
	tileTexture = NULL;
}


void Tile::updateTile()
{
	float lerpSpeed = 0.08;

	// Update x
	if (x != goalX)
	{
		xLerp = 0.0;
	}

	if (xLerp < 1.0)
	{
		xLerp += lerpSpeed;
	}

	float newX = lerp(x, xLerp, goalX);
	x = newX;

	// Update y
	if (y != goalY)
	{
		yLerp = 0.0;
	}

	if (yLerp < 1.0)
	{
		yLerp += lerpSpeed;
	}

	float newY = lerp(y, yLerp, goalY);
	y = newY;

	// Update z
	if (z != goalZ)
	{
		zLerp = 0.0;
	}

	if (zLerp < 1.0)
	{
		zLerp += lerpSpeed;
	}

	float newZ = lerp(z, zLerp, goalZ);

	z = newZ;

	additionalAnimation();
}


bool Tile::isMovementAllowed()
{
	// Check if the tile has waited long enough
	// to move
	if (SDL_GetTicks() - prevMovementTime >= movementWaitTime)
	{
		prevMovementTime = SDL_GetTicks();
		return true;
	}
	else
	{
		return false;
	}
}


void Tile::draw(SDL_Renderer* renderer)
{
	// Don't draw empty tiles
	if (empty == false)
	{
		SDL_RenderCopy(renderer, tileTexture, NULL, &getRect());
	}
}


void Tile::additionalAnimation()
{
	// No additional animation for basic tiles
}


SDL_Rect Tile::getRect()
{
	SDL_Rect tileRect = {x, y, width*z+1, height*z+1};
	return tileRect;
}


float Tile::lerp(float x, float t, float y)
{
	return x * (1-t) + y*t;
}