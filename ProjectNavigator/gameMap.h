// FILE: gameMap.h
// PROG: Dylan Gallardo	
// PURP: Header for gameMap.cpp

#ifndef GAME_MAP_H
#define GAME_MAP_H

class GameMap
{
	public:
		GameMap();
		~GameMap();
		bool generateNewMap(int layers, int layerRowAmt, int layerColumnAmt,
			double tileWidth, double tileHeight, SDL_Renderer*);
		bool loadMap(std::string fileLocation,  SDL_Renderer*);
		bool saveMap(std::string fileLocation);
		void updateMap();
		void drawMap(SDL_Rect screenRect, SDL_Renderer* renderer);

		bool moveUp, moveDown, moveLeft, moveRight;

		double getX() {return x;}
		double getY() {return y;}
		double getTileW() {return tileW;}
		double getTileH() {return tileH;}
		int getCenterLayer() {return centerLayer;}
		int getLayerCount() {return layerCount;}

		void setX(double newX) {x = newX;}
		void setY(double newY) {y = newY;}
		void setCenterLayer(int newLayer) {centerLayer = newLayer;}
		
	private:
		std::vector<SDL_Texture*> loadTileSheet(const char*, SDL_Renderer*);
		SDL_Texture* createTextureFromSurface(std::string, SDL_Surface*, SDL_Renderer* renderer);
		int convertStringToInt(std::string);
		float randomNumber(float, float);

		std::vector<TileMap*> layers;
		std::string mapFileName;
		std::vector<SDL_Texture*> tileTextures;
		double x, y;
		double tileW, tileH;
		int centerLayer;
		int layerCount;
		int maxLayersDisplayed;
};

#endif