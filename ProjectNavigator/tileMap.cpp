//tileMap.cpp - source file for the game's tile map. 

#include <SDL.h>
#include <fstream> //is a part of <stdio>?
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>

#include "tile.h"
#include "tileMap.h"

using namespace std;
extern double zoom;


TileMap::TileMap()
{

}


TileMap::~TileMap()
{

}


bool TileMap::generateMap(int rowAmt, int columnAmt, double tileWidth, double tileHeight, 
						 std::vector<SDL_Texture*> tileTextures)
{
	x = 0;
	y = 0;
	rows = rowAmt;
	columns = columnAmt;
	tileH = tileHeight;
	tileW = tileWidth;

	float tempX = x;
	float tempY = y;

	for (int r=0; r<rows; r++)
	{
		// Add a new row of tiles
		vector<Tile*> newRow;
		for (int i=0; i<columns; i++)
		{
			newRow.push_back(new Tile);
		}
		tiles.push_back(newRow);

		// Set tile in each column
		for (int c=0; c<columns; c++)
		{
			float tileX = tempX + c*tileW;
			tiles[r][c]->setX(tileX);
			tiles[r][c]->setY(tempY);
			tiles[r][c]->setZ(1);
			tiles[r][c]->setWidth(tileW);
			tiles[r][c]->setHeight(tileH);

			// Create ground level tiles
			if (r == 0)
			{
				int randTexture = randomNumber(0, 8);
				tiles[r][c]->setTileTexture(tileTextures[randTexture]);
				switch (randTexture)
				{
					case 0:
					case 1:
					case 2:
						tiles[r][c]->setType(tiles[r][c]->dirt);
						break;
					case 3:
					case 4:
					case 5:
						tiles[r][c]->setType(tiles[r][c]->grey);
					case 6:
					case 7:
					case 8:
						tiles[r][c]->setType(tiles[r][c]->orange);
				}
			}
			// Cactus time
			else if (randomNumber(0, 100) < 1 && tiles[r-1][c]->getEmpty() == false)
			{
				tiles[r][c]->setTileTexture(tileTextures[randomNumber(9, 12)]);
				tiles[r][c]->setType(tiles[r][c]->cactus);
			}
			else
			{
				tiles[r][c]->setEmpty(true);
			}
		}

		//move to next row
		tempX = x;
		tempY += tileH;
	}

	return true;
} // END generateMap


bool TileMap::loadMap(vector<string> mapDataStr)
{
	return false;
} // END loadMap


void TileMap::saveMapFile(std::ofstream& externMapFile)
{
	//externMapFile.open("mapFile.txt");

	for (int r=0; r<rows; r++)
	{
		for (int c=0; c<columns; c++)
		{
			externMapFile << tiles[r][c]->getType() << ",";
		}

		externMapFile << endl;
	}
}


void TileMap::updateTiles()
{
	double tempX = x;
	double tempY = y;

	// Update tiles in reverse, so that they stack as expected
	// (not ideal, I'd prefer changing this later)
	for (int r=0; r < rows; r++)
	{
		// Set tile in each column
		for (int c=0; c<columns; c++)
		{
			if (tiles[r][c]->getEmpty() == false) // Don't update empty tiles
			{
				tiles[r][c]->setX(tempX + (tileW + (tileW * z) * c));
				tiles[r][c]->setY(tempY);
				tiles[r][c]->setZ(z);
				tiles[r][c]->updateTile();
			}
		}

		tempY -= tileH*z;
	}
} // END updateTiles()


void TileMap::replaceTile(Tile* newTile, int row, int column)
{
	tiles[row][column] = newTile;
}


void TileMap::setTileEmpty(int row, int column)
{
	tiles[row][column] = new Tile();
	tiles[row][column]->setEmpty(true);
}


void TileMap::drawTileMap(SDL_Rect screenRect, SDL_Renderer *renderer)
{
	// Draw elements
	for (int r=0; r<rows; r++)
	{
		for (int c=0; c<columns; c++)
		{
			tiles[r][c]->draw(renderer);
		}
	}
} // END draw()


// Untested!!!
int TileMap::convertStringToInt(string stringIn)
{
	return atoi(stringIn.c_str());
}


float TileMap::randomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}