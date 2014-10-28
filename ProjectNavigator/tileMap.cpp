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
	for (int i=0; i<tiles.size(); i++)
	{
		tiles[i]->~Tile();
	}
}


void TileMap::initialize(std::string fileLocation, int rowAmt, int columnAmt, double tileHeight, double tileWidth)
{
	tiles[0];
	mapFileName = fileLocation;
	rows = rowAmt;
	columns = columnAmt;
	tileH = tileHeight;
	tileW = tileWidth;
	//renderer = &mainRenderer;

	// Try to load map, otherwise generate a new one
	if (!loadMapFile(mapFileName))
	{
		//generate new map
		for (int c=0; c<=columns*rows; c++)
		{
			tileMap.push_back(randomNumber(1, 5)); //remember rand(1, 5) = values 1-4 NOT values 1-5
		}
	}
}


void TileMap::drawTileMap(SDL_Rect screenRect, SDL_Renderer *renderer)
{
	updateTileRects(); // Temporary, shouldn't call this from draw()

	vector<int> tilesToDraw;
	//tilesToDraw = getTilesInRect(screenRect);

	for (int i=0; i<tileMap.size()-1; i++)
	{
		tilesToDraw.push_back(i);
	}

	if (tilesToDraw.size() <= 0)
	{
		return;
	}

	// Draw elements
	for (int i=tilesToDraw.front(); i<tilesToDraw.size(); i++)
	{
			if (tileMap[i] == 1)
			{
				SDL_RenderCopy(renderer, tiles[0]->gettileTexture(), NULL, &tileRects[i]);
			}
			else if (tileMap[i] == 2)
			{
				SDL_RenderCopy(renderer, tiles[1]->gettileTexture(), NULL, &tileRects[i]);
			}
			else if (tileMap[i] == 3)
			{
				SDL_RenderCopy(renderer, tiles[2]->gettileTexture(), NULL, &tileRects[i]);
			}
			else
			{
				SDL_RenderCopy(renderer, tiles[3]->gettileTexture(), NULL, &tileRects[i]);
			}
	}

} // END draw()


void TileMap::saveMapFile()
{
	ofstream externMapFile;
	externMapFile.open("mapFile.txt");

	for (int i=0; i<tileMap.size(); i++)
	{
		//produces a non-human readable mess... add in a way to read 
		//tileMap[i]+' '; later so that the output file is readable by a person
		externMapFile << tileMap[i] << ' ';
	}
	
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
	}
	else
	{
		// Couldn't read file
		return false;
	}

	//"tileSheet.bmp" should be a parameter in the init function
	//to make this class more versatile
	tileSheet = SDL_LoadBMP("tileSheet.bmp");

	//load traits from txt file.
	//"tileTraits.txt" should be a parameter in the init function
	ifstream tileTraitFile;
	tileTraitFile.open ("tileTraits.txt");

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

	//make all recently read tiles into textures and free surfaces
	for (int i=0; i<tileSurface.size(); i++)
	{
		SDL_Texture *tempTexture = loadTexture("emptyString", tileSurface[i], renderer);
		tiles.push_back(new Tile);
		tiles.back()->settileTexture(tempTexture);

		string traitLine;

		if (tileTraitFile.is_open())
		{
			getline(tileTraitFile, traitLine);
		}
		else cout << "Unable to open file";

		tiles.back()->setCollision(traitLine[0] - '0');
		tiles.back()->setTrait1(traitLine[1] - '0');

		//use this when you expand the functionality to handle longer digits than 0-9
		//cout << atoi(traitLine.c_str()) << endl << endl;
	}

	tileTraitFile.close();

	for (int i=tileSurface.size()-1; i<=0; i=tileSurface.size()-1)
	{
		SDL_FreeSurface(tileSurface[i]);
	}
}


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