// FILE: gameMap.cpp
// PROG: Dylan Gallardo	
// PURP: Creates the psuedo-3D layered map for the game. Each layer consists of a
//		 TileMap class, which stores and tracks info about each layer's individual tiles.
//		 the GameMap class updates each layer and provides functions that allow easy access
//		 to specific layer and tile properties.

#include <SDL.h>
#include <SDL_image.h>
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
#include "gameMap.h"

using namespace std;


GameMap::GameMap()
{
	mapFileName = "null";
	x = 0; 
	y = 0;
	tileW = 5;
	tileH = 5;
	centerLayer = 0;
	layerCount = 0;
	layerSpacing = 40;

	moveUp = false;
	moveDown = false;
	moveLeft = false;
	moveRight = false;
}


GameMap::~GameMap()
{

}


bool GameMap::generateNewMap(int layerCountIn, int layerRowAmt, int layerColumnAmt,
			double tileWidth, double tileHeight, SDL_Renderer* renderer)
{
	tileTextures = loadTileSheet("tileSheet.png", renderer); //test for errors
	x = -1700;
	y = 200;
	float tempX = x;
	float tempY = y;
	tileW = tileWidth;
	tileH = tileHeight;
	layerCount = layerCountIn;
	centerLayer = layerCount/2;
	maxLayersDisplayed = 100;

	for (int i=0; i<layerCount; i++)
	{
		// Add a new layer
		TileMap* newLayer = new TileMap();
		newLayer->generateMap(layerRowAmt, layerColumnAmt, tileWidth, tileHeight, tileTextures);
		newLayer->setX(x);
		newLayer->setY(y);
		layers.push_back(newLayer);

		//move to next row
		tempX = x;
		tempY += tileH;
	}

	return true;
} // END generateNewMap()


bool GameMap::loadMap(std::string filePath, SDL_Renderer* renderer)
{
	tileTextures = loadTileSheet("tileSheet.png", renderer);

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
} // END loadMap()


bool GameMap::saveMap(std::string fileLocation)
{
	return false;
}


void GameMap::updateMap()
{
	// Movement position changes
	if (moveUp)
	{
		if (centerLayer > 0)
		{
			centerLayer --;
		}
	}
	else if (moveDown)
	{
		if (centerLayer < layerCount-1)
		{
			centerLayer ++;
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

	//cout << centerLayer << endl;
	layers[centerLayer]->setX(x);
	

	int startLayer = centerLayer-(maxLayersDisplayed/2);
	if (startLayer < 0) {startLayer = 0;}
	int endLayer = startLayer + maxLayersDisplayed;
	if (endLayer > layerCount) {endLayer = layerCount;}

	float originalWidth = layers[centerLayer]->getX() + tileW * layers[centerLayer]->getColumnCount();

	// Updating tilemap positions
	for (int i=startLayer; i<endLayer; i++)
	{
		float distFromCenter = i - centerLayer;

		//layers[i]->setY(layers[i]->getY() + distFromCenter * (distFromCenter/2));
		layers[i]->setY(y + distFromCenter * (distFromCenter/2));

		distFromCenter /= layerSpacing;
		float newWidth = x + (tileW*distFromCenter) * layers[centerLayer]->getColumnCount();
		float newX = x - ((newWidth - originalWidth)/2);

		layers[i]->setX(newX);
		layers[i]->setZ(1+distFromCenter);

		layers[i]->updateTiles();
	}
} // END updateMap()


void GameMap::drawMap(SDL_Rect screenRect, SDL_Renderer* renderer)
{
	int startLayer = centerLayer-(maxLayersDisplayed/2);
	if (startLayer < 0) {startLayer = 0;}
	int endLayer = startLayer + maxLayersDisplayed;
	if (endLayer > layers.size()) {endLayer = layers.size();}

	for (int i=startLayer; i<endLayer; i++)
	{
		layers[i]->drawTileMap(screenRect, renderer);
	}
} // END drawMap()


std::vector<SDL_Texture*> GameMap::loadTileSheet (const char* tileSheetPath, SDL_Renderer* renderer)
{
	SDL_Surface* tileSheet = IMG_Load(tileSheetPath);
	
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
			SDL_Surface *tempSurface = IMG_Load("tempTile.png");
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
		SDL_Texture *tempTexture = createTextureFromSurface("emptyString", tileSurface[i], renderer);
		tileTextures.push_back(tempTexture);
	}

	for (int i=tileSurface.size()-1; i<=0; i=tileSurface.size()-1)
	{
		SDL_FreeSurface(tileSurface[i]);
	}

	return tileTextures;
} // END loadTileTexture()


SDL_Texture* GameMap::createTextureFromSurface (std::string path, SDL_Surface *currentSurface, SDL_Renderer *renderer)
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


void GameMap::randomizeLayerSpacing()
{
	layerSpacing = randomNumber(abs(layerSpacing-10), layerSpacing+10);
}


// Untested!!!
int GameMap::convertStringToInt(string stringIn)
{
	return atoi(stringIn.c_str());
}


float GameMap::randomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}