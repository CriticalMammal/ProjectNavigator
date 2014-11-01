#include <SDL.h>

#include "tile.h"

using namespace std;


Tile::Tile()
{
	collision = false;
	type = None;
	tileTexture = NULL;

	x = 0;
	y = 0;
	z = 0;
	
	width = 5;
	height = 5;
}


Tile::~Tile()
{
	SDL_DestroyTexture(tileTexture);
	tileTexture = NULL;
}


SDL_Rect Tile::getRect()
{
	SDL_Rect tileRect = {x, y, width, height};
	return tileRect;
}