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
}

Tile::~Tile()
{
	SDL_DestroyTexture(tileTexture);
	tileTexture = NULL;
}