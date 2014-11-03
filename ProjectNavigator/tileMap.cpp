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


void TileMap::initialize(std::string fileLocation, int rowAmt, int columnAmt, double tileWidth, double tileHeight, 
						 SDL_Renderer* renderer)
{
	mapFileName = fileLocation;
	x = (columnAmt*tileWidth)/8.2 - (columnAmt*tileWidth);
	oldX = x;
	y = 150;
	rows = rowAmt;
	columns = columnAmt;
	tileH = tileHeight;
	tileW = tileWidth;
	centerRow = rows/2;
	maxRowsDisplayed = 200;

	moveUp = false;
	moveDown = false;
	moveRight = false;
	moveLeft = false;

	// Try to load map, otherwise generate a new one
	if (!loadMapFile(mapFileName))
	{
		vector<SDL_Texture*> tileTextures = loadTileSheet("tileSheet.bmp", renderer);
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
				tiles[r][c]->setTileTexture(tileTextures[randomNumber(0, 890)/100]);
			}

			//move to next row
			tempX = x;
			tempY += tileH;
		}
	}
}


void TileMap::updateTiles()
{
	//x = oldX;
	if (moveUp)
	{
		if (centerRow > 0)
		{
			centerRow --;
		}
	}
	else if (moveDown)
	{
		if (centerRow < rows)
		{
			centerRow ++;
		}
	}

	if (moveLeft)
	{
		x += tileW + (tileW*0.525);
	}
	else if (moveRight)
	{
		x -= tileW + (tileW*0.525);
	}
	
	int startRow = centerRow-(maxRowsDisplayed/2);
	if (startRow < 0) {startRow = 0;}
	int endRow = startRow + maxRowsDisplayed;
	if (endRow > rows) {endRow = rows;}

	float originalWidth = x + tileW*columns;
	int sizeFactor = 40;

	for (int r=startRow; r<endRow; r++)
	{
		float distFromCenter = r - centerRow;
		distFromCenter /= sizeFactor;
		float newWidth = x + (tileW*distFromCenter)*columns;
		float newX = x - ((newWidth - originalWidth)/2);

		// Set tile in each column
		for (int c=0; c<columns; c++)
		{
			// Calculate what the z axis should be
			distFromCenter = r-centerRow;
			
			tiles[r][c]->setY(y+(distFromCenter*(distFromCenter/2)));

			distFromCenter = distFromCenter/sizeFactor;
			tiles[r][c]->setZ(1+distFromCenter);
			tiles[r][c]->setX(newX + (tileW+(tileW*distFromCenter))*c);

			tiles[r][c]->updateTile();
		}
	}
}


void TileMap::drawTileMap(SDL_Rect screenRect, SDL_Renderer *renderer)
{
	int startRow = centerRow-(maxRowsDisplayed/2);
	if (startRow < 0) {startRow = 0;}
	int endRow = startRow + maxRowsDisplayed;
	if (endRow > rows) {endRow = rows;}

	// Draw elements
	for (int r=startRow; r<endRow; r++)
	{
		for (int c=0; c<tiles[r].size(); c++)
		{
			SDL_RenderCopy(renderer, tiles[r][c]->gettileTexture(), NULL, &tiles[r][c]->getRect());
		}
	}

} // END draw()


void TileMap::saveMapFile()
{
	ofstream externMapFile;
	externMapFile.open("mapFile.txt");

	/*
	for (int i=0; i<tileMap.size(); i++)
	{
		//produces a non-human readable mess... add in a way to read 
		//tileMap[i]+' '; later so that the output file is readable by a person
		externMapFile << tileMap[i] << ' ';
	}
	*/

	externMapFile.close();
}


bool TileMap::loadMapFile(string filePath)
{
	ifstream mapFile(filePath);
	
	if (mapFile.is_open())
	{
		// You'll need to seperate most of this into something like
		// the CabRecordReaper, to simplify this section and allow
		// you to read each line and create tiles from them.
		std::vector<int> tileIntLine;

		// Read a line of the file
		while (mapFile)
		{
			string recordLine;
			
			if (!getline( mapFile, recordLine))
			{
				break;
			}

			// Break up line into parts
			istringstream strStream(recordLine);

			while(strStream)
			{
				string recordPart;
				if (!getline(strStream, recordPart, ','))
				{
					break;
				}
				
				// Create a convertStringToInt function
				int partValue = convertStringToInt(recordPart);
				tileIntLine.push_back(partValue);
			}
		}

		mapFile.close();
		return true;
	}
	else
	{
		// Couldn't read file
		return false;
	}
}


std::vector<SDL_Texture*> TileMap::loadTileSheet (const char* tileSheetPath, SDL_Renderer* renderer)
{
	tileSheet = SDL_LoadBMP(tileSheetPath); //"tileSheet.bmp"
	
	vector<SDL_Surface*> tileSurface;

	//the actual pixel width/height of tiles in tilesheet image
	int importHeight = 16;
	int importWidth = 16;

	//load the tile sheet into seperate surfaces
	for (int h=0; h<tileSheet->h; h+=importHeight)
	{
		for (int w=0; w<tileSheet->w; w+=importWidth)
		{
			// You needed to load a temporary tile in order for
			SDL_Surface *tempSurface = SDL_LoadBMP("tempTile.bmp");
			tileSurface.push_back(tempSurface);

			SDL_Rect clip = {w, h, importWidth, importHeight};

			if (SDL_BlitSurface(tileSheet, &clip, tileSurface.back(), NULL) != 0)
			{
				cout << "Error, SDL_Blit failed: " << SDL_GetError() << endl;
			}
		}
	}

	vector<SDL_Texture*> tileTextures;

	//make all recently read tiles into textures and free surfaces
	for (int i=0; i<tileSurface.size(); i++)
	{
		SDL_Texture *tempTexture = loadTexture("emptyString", tileSurface[i], renderer);
		tileTextures.push_back(tempTexture);
	}

	for (int i=tileSurface.size()-1; i<=0; i=tileSurface.size()-1)
	{
		SDL_FreeSurface(tileSurface[i]);
	}

	return tileTextures;

} // END loadTileTexture()


SDL_Texture* TileMap::loadTexture (std::string path, SDL_Surface *currentSurface, SDL_Renderer *renderer)
{
	//final image
	SDL_Texture* newTexture = nullptr;

	//Load Image at specified path OR use currentSurface if available
	SDL_Surface* loadedSurface;

	if (!currentSurface)
	{
		loadedSurface = SDL_LoadBMP(path.c_str());
	}
	else
	{
		loadedSurface = currentSurface;
	}
	
	if (!loadedSurface)
	{
		cout << "Failed to load image " << path.c_str() << ". SDL_Error: " << SDL_GetError() << endl;
	}
	else
	{
		//convert surface to screen format
		if (!(newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface)))
		{
			cout << "Failed to create texture " << path.c_str() << ". SDL_Error: " << SDL_GetError() << endl;
		}

		//free old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
} //END loadSurface()


// Untested!!!
int TileMap::convertStringToInt(string stringIn)
{
	return atoi(stringIn.c_str());
}


float TileMap::randomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}