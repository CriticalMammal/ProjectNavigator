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
#include "playerTile.h"
#include "tileMap.h"
#include "gameMap.h"

using namespace std;


GameMap::GameMap()
{
	mapFileName = "null";
	xOffset = 0;
	yOffset = 0;
	x = 0; 
	y = 0;
	z = 1;
	tileW = 5;
	tileH = 5;
	centerLayer = 0;
	layerCount = 0;
	layerSpacing = 40;
	playerCloseness = 20;
	editMode = false;

	moveUp = false;
	moveDown = false;
	moveLeft = false;
	moveRight = false;
	moveForward = false;
	moveBackward = false;
}


GameMap::~GameMap()
{
	// Delete pointers

}


bool GameMap::generateNewMap(int layerCountIn, int layerRowAmt, int layerColumnAmt,
			double tileWidth, double tileHeight, SDL_Renderer* renderer)
{
	tileTextures = loadTileSheet("tileSheet.png", renderer); //test for errors
	xOffset = -100;
	yOffset = 100;
	tileW = tileWidth;
	tileH = tileHeight;
	layerCount = layerCountIn;
	centerLayer = layerCount/2;
	maxLayersDisplayed = 80;

	{
		playerCloseness = 20;
		Player* playerTile = new Player();
		playerTile->setTileTexture(tileTextures[6]);
		playerTile->setWidth(tileW);
		playerTile->setHeight(tileH);
		playerTile->setType(playerTile->player);

		focusLocation.tile = playerTile;
		focusLocation.layer = centerLayer + playerCloseness;
		focusLocation.row = 1; //on top of ground level
		focusLocation.column = layerColumnAmt/2; // middle column
	}

	for (int i=0; i<layerCount; i++)
	{
		// Add a new layer
		TileMap* newLayer = new TileMap();
		newLayer->generateMap(layerRowAmt, layerColumnAmt, tileWidth, tileHeight, tileTextures);
		newLayer->setX(x);
		newLayer->setY(y);

		if (i == centerLayer + playerCloseness) // Add player tile
		{
			newLayer->replaceTile(focusLocation.tile, focusLocation.row, focusLocation.column);
		}

		layers.push_back(newLayer);
	}

	return true;
} // END generateNewMap()


bool GameMap::loadMap(std::string filePath, SDL_Renderer* renderer)
{
	tileTextures = loadTileSheet("tileSheet.png", renderer);
	xOffset = -100;
	yOffset = 100;

	ifstream mapFile(filePath);
	
	if (mapFile.is_open())
	{
		// First get game map's dimensions
		int layerRowCount = 0;
		int layerColumnCount = 0;

		if (mapFile.good())
		{
			string mapData;
			getline(mapFile, mapData, ',');
			layerCount = convertStringToInt(mapData);

			getline(mapFile, mapData, ',');
			layerRowCount = convertStringToInt(mapData);

			getline(mapFile, mapData, ',');
			layerColumnCount = convertStringToInt(mapData);

			getline(mapFile, mapData, ',');
			tileW = convertStringToInt(mapData);

			getline(mapFile, mapData, ',');
			tileH = convertStringToInt(mapData);

			getline(mapFile, mapData); // there was an empty line of data for some reason
		}

		// Init other variables
		centerLayer = layerCount/2;
		maxLayersDisplayed = 80;
		playerCloseness = 20;
		
		// Initialize each game map layer
		for (int i=0; i<layerCount; i++)
		{
			// Two dimensional vector to store map data
			std::vector< std::vector<int> > layerMapData;

			for (int r=0; r<layerRowCount; r++)
			{
				vector<int> temp;
				layerMapData.push_back(temp);

				// Read a line of the file
				if (mapFile.good())
				{
					string recordLine;
					getline(mapFile, recordLine);

					// Break up line into parts
					istringstream strStream(recordLine);
					while(strStream.good())
					{
						string recordPart;
						getline(strStream, recordPart, ',');
						
						// Add valid number to vector
						if (recordPart[0] != '*' && recordPart.empty() == false)
						{
							int partValue = convertStringToInt(recordPart);
							layerMapData[r].push_back(partValue);
						}
					}
				}
			}

			// Create map layer from data just read
			TileMap* newLayer = new TileMap();
			newLayer->loadMap(layerMapData, tileH, tileW, tileTextures);
			newLayer->setX(x);
			newLayer->setY(y);

			layers.push_back(newLayer);
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
	ofstream externMapFile;
	externMapFile.open(fileLocation);

	// Write some map information
	// layerCount, layerRowCount, layerColumnCount, layerTileW, layerTileH
	externMapFile << layers.size() << "," << layers[0]->getRowCount() << "," << 
		layers[0]->getColumnCount() << "," << layers[0]->getTileW() << "," << 
		layers[0]->getTileH() << "," << endl;

	// Save Tile Types for each layer
	for (int i=0; i<layers.size(); i++)
	{
		layers[i]->saveMapFile(externMapFile);
	}

	externMapFile.close();

	return true;
}


void GameMap::updateMap()
{
	// The way you check for this needs to be changed, since the initialized values
	// are unpredictable from the Debug to Release versions of the program.
	// Maybe just use a bool or something...
	if (focusLocation.column < -10000 || focusLocation.column > 10000)
	{
		findPlayerTile();
	}
	
	moveFocusForward();
	moveFocusBackward();
	moveFocusUp();
	moveFocusDown();
	moveFocusLeft();
	moveFocusRight();
	
	layers[centerLayer]->setX(x);
	
	int startLayer = focusLocation.layer - (maxLayersDisplayed/2);
	if (startLayer < 0) {startLayer = 0;}
	int endLayer = startLayer + maxLayersDisplayed;
	if (endLayer > layers.size()) {endLayer = layers.size();}

	float originalWidth = layers[centerLayer]->getX() + tileW * layers[centerLayer]->getColumnCount();

	// Updating tilemap positions
	for (int i=startLayer; i<endLayer; i++)
	{
		float distFromCenter = i - centerLayer;
		layers[i]->setY(y + distFromCenter * (distFromCenter/2));
		
		distFromCenter /= layerSpacing;
		
		float newWidth = x + (tileW*distFromCenter) * layers[centerLayer]->getColumnCount();
		float newX = x - ((newWidth - originalWidth)/2);

		layers[i]->setX(newX);
		layers[i]->setZ(z+distFromCenter);

		layers[i]->updateTiles();
	}

	updateFocusTile();
	
	// Update map based on focus position (Should you really be doing this again in focus movement?)
	x = xOffset - focusLocation.column*(tileW*focusLocation.tile->getZ());
	y = yOffset + focusLocation.row * (tileH * focusLocation.tile->getZ());
	centerLayer = focusLocation.layer-playerCloseness;
	if (centerLayer <= 0)
	{
		centerLayer = 0;
	}
	
} // END updateMap()


// Used if player tile location is unknown, doesn't do any error checks
void GameMap::findPlayerTile()
{
	for (int i=0; i<layers.size(); i++)
	{
		TileMap::TileInfo playerInfo = layers[i]->findPlayerTile();
		focusLocation.tile = playerInfo.tile;

		if (focusLocation.tile != nullptr)
		{
			// Update position info
			focusLocation.row = playerInfo.row;
			focusLocation.column = playerInfo.column;
			focusLocation.layer = i;
			return;
		}
		else
		{
			focusLocation.tile = new Player();
			focusLocation.tile->setHeight(tileH);
			focusLocation.tile->setWidth(tileW);
			focusLocation.tile->setType(Tile::player);
		}
	}

	return;
}


void GameMap::moveFocusForward()
{
	if (moveForward == true && focusLocation.tile->canMove())
	{
		if (editMode == false && 
			layers[focusLocation.layer-1]->isTileEmpty(focusLocation.row, focusLocation.column))
		{
			layers[focusLocation.layer]->setTileEmpty(focusLocation.row, focusLocation.column);
			focusLocation.layer -= 1;
			if (focusLocation.layer-1 <= 0)
			{
				focusLocation.layer = layers.size()-2;
			}
			layers[focusLocation.layer]->replaceTile(focusLocation.tile ,focusLocation.row, focusLocation.column);
		}
		else if (editMode == true)
		{
			focusLocation.tile->setIsEditTile(false);
			focusLocation.layer -= 1;
			if (focusLocation.layer-1 <= 0)
			{
				focusLocation.layer = layers.size()-2;
			}
			focusLocation.tile = layers[focusLocation.layer]->getTileAt(focusLocation.row, focusLocation.column);
			focusLocation.tile->setIsEditTile(true);
			focusLocation.tile->canMove(); // Reset the movement time
		}
		/*
		centerLayer = focusLocation.layer-playerCloseness;
		if (centerLayer <= 0)
		{
			centerLayer = 0;
		}
		*/
	}
}


void GameMap::moveFocusBackward()
{
	if (moveBackward == true && focusLocation.tile->canMove())
	{
		if (editMode == false && 
			layers[focusLocation.layer+1]->isTileEmpty(focusLocation.row, focusLocation.column))
		{
			layers[focusLocation.layer]->setTileEmpty(focusLocation.row, focusLocation.column);
			focusLocation.layer += 1;
			if (focusLocation.layer >= layers.size()-1)
			{
				focusLocation.layer = 1;
			}
			layers[focusLocation.layer]->replaceTile(focusLocation.tile ,focusLocation.row, focusLocation.column);
		}
		else if (editMode == true)
		{
			focusLocation.tile->setIsEditTile(false);
			focusLocation.layer += 1;
			if (focusLocation.layer >= layers.size()-1)
			{
				focusLocation.layer = 1;
			}
			focusLocation.tile = layers[focusLocation.layer]->getTileAt(focusLocation.row, focusLocation.column);
			focusLocation.tile->setIsEditTile(true);
			focusLocation.tile->canMove(); // Reset the movement time
		}
		/*
		centerLayer = focusLocation.layer-playerCloseness;
		if (centerLayer <= 0)
		{
			centerLayer = 0;
		}
		*/
	}
}


void GameMap::moveFocusLeft()
{
	if (moveLeft == true && focusLocation.column-1 >= 0 && 
		focusLocation.tile->canMove())
	{
		if (editMode == false && 
			layers[focusLocation.layer]->isTileEmpty(focusLocation.row, focusLocation.column-1))
		{
			layers[focusLocation.layer]->setTileEmpty(focusLocation.row, focusLocation.column);
			focusLocation.column -= 1;
			layers[focusLocation.layer]->replaceTile(focusLocation.tile, focusLocation.row, focusLocation.column);
		}
		else if (editMode == true)
		{
			focusLocation.tile->setIsEditTile(false);
			focusLocation.column -= 1;
			focusLocation.tile = layers[focusLocation.layer]->getTileAt(focusLocation.row, focusLocation.column);
			focusLocation.tile->setIsEditTile(true);
			focusLocation.tile->canMove(); // Reset the movement time
		}

		x = xOffset - focusLocation.column*(tileW*focusLocation.tile->getZ());
	}
}


void GameMap::moveFocusRight()
{
	if (moveRight == true && focusLocation.column+1 < layers[focusLocation.layer]->getColumnCount()
		 && focusLocation.tile->canMove())
	{
		if (editMode == false && 
			layers[focusLocation.layer]->isTileEmpty(focusLocation.row, focusLocation.column+1))
		{
			layers[focusLocation.layer]->setTileEmpty(focusLocation.row, focusLocation.column);
			focusLocation.column += 1;
			layers[focusLocation.layer]->replaceTile(focusLocation.tile, focusLocation.row, focusLocation.column);
		}
		else if (editMode == true)
		{
			focusLocation.tile->setIsEditTile(false);
			focusLocation.column += 1;
			focusLocation.tile = layers[focusLocation.layer]->getTileAt(focusLocation.row, focusLocation.column);
			focusLocation.tile->setIsEditTile(true);
			focusLocation.tile->canMove(); // Reset the movement time
		}

		x = xOffset - focusLocation.column*(tileW*focusLocation.tile->getZ());
	}
}


void GameMap::moveFocusUp()
{
	if (moveUp == true && focusLocation.row+1 < layers[focusLocation.layer]->getRowCount())
	{
		if (editMode == false && 
			layers[focusLocation.layer]->isTileEmpty(focusLocation.row+1, focusLocation.column) &&
			focusLocation.tile->canJump())
		{
			layers[focusLocation.layer]->setTileEmpty(focusLocation.row, focusLocation.column);
			focusLocation.row += 1;
			layers[focusLocation.layer]->replaceTile(focusLocation.tile, focusLocation.row, focusLocation.column);
			focusLocation.tile->canFall(); // Reset falling time
		}
		else if (editMode == true && focusLocation.tile->canMove())
		{
			focusLocation.tile->setIsEditTile(false);
			focusLocation.row += 1;
			focusLocation.tile = layers[focusLocation.layer]->getTileAt(focusLocation.row, focusLocation.column);
			focusLocation.tile->setIsEditTile(true);
			focusLocation.tile->canMove(); // Reset the movement time
		}

		y = yOffset + focusLocation.row * (tileH * focusLocation.tile->getZ());
	}
}


void GameMap::moveFocusDown()
{
	if (moveDown == true && focusLocation.row-1 >= 0 && 
		focusLocation.tile->canMove())
	{
		if (editMode == false && 
			layers[focusLocation.layer]->isTileEmpty(focusLocation.row-1, focusLocation.column))
		{
			layers[focusLocation.layer]->setTileEmpty(focusLocation.row, focusLocation.column);
			focusLocation.row -= 1;
			layers[focusLocation.layer]->replaceTile(focusLocation.tile, focusLocation.row, focusLocation.column);
		}
		else if (editMode == true)
		{
			focusLocation.tile->setIsEditTile(false);
			focusLocation.row -= 1;
			focusLocation.tile = layers[focusLocation.layer]->getTileAt(focusLocation.row, focusLocation.column);
			focusLocation.tile->setIsEditTile(true);
			focusLocation.tile->canMove(); // Reset the movement time
		}

		y = yOffset + focusLocation.row * (tileH * focusLocation.tile->getZ());
	}
}


void GameMap::updateFocusTile()
{
	// If in edit mode, don't use gravity or if on bottom
	// level of the map
	if (focusLocation.tile->getIsEditTile() == true ||
		focusLocation.row-1 < 0)
	{
		return;
	}

	//Perhaps if you check periodically for the focus tile you
	// wouldn't need to specifically update the focus tile
	// Updates focus tile's gravity
	Tile *tileBelowFocus = layers[focusLocation.layer]->getTileAt(focusLocation.row-1, focusLocation.column);

	if (focusLocation.row-1 >= 0 && tileBelowFocus->getEmpty() == true &&
		focusLocation.tile->canFall())
	{
		layers[focusLocation.layer]->setTileEmpty(focusLocation.row, focusLocation.column);
		focusLocation.row --;
		layers[focusLocation.layer]->replaceTile(focusLocation.tile, focusLocation.row, focusLocation.column);
		y = yOffset + focusLocation.row * (tileH * focusLocation.tile->getZ());
	}
}


void GameMap::setFocusTile(int tileType)
{
	Tile* tempTile = new Tile();
	if (tileType < tileTextures.size())
	{
		tempTile->setType((Tile::TileType) tileType);
		tempTile->setTileTexture(tileTextures[tileType]);
	}
	else
	{
		tempTile->setType(Tile::none);
		tempTile->setEmpty(true);
	}

	tempTile->setX(0);
	tempTile->setY(0);
	tempTile->setWidth(tileW);
	tempTile->setHeight(tileH);
	
	layers[focusLocation.layer]->replaceTile(tempTile, focusLocation.row, focusLocation.column);
}


void GameMap::drawMap(SDL_Rect screenRect, SDL_Renderer* renderer)
{
	int startLayer = focusLocation.layer - (maxLayersDisplayed/2);
	if (startLayer < 0) {startLayer = 0;}
	int endLayer = startLayer + maxLayersDisplayed;
	if (endLayer > layers.size()) {endLayer = layers.size();}

	for (int i=startLayer; i<endLayer; i++)
	{
		layers[i]->drawTileMap(screenRect, renderer);
		if (editMode == true && focusLocation.layer == i)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 70);
			SDL_RenderDrawRect(renderer, &focusLocation.tile->getRect());
			SDL_RenderFillRect(renderer, &focusLocation.tile->getRect());
		}
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