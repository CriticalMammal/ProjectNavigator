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
		void randomizeLayerSpacing();
		void findPlayerTile();
		void moveFocusRight();
		void moveFocusLeft();
		void moveFocusForward();
		void moveFocusBackward();
		void moveFocusUp();
		void moveFocusDown();
		void setFocusTile(int tileType);

		struct tileLocation
		{
			Tile* tile;
			int layer, row, column;
		};

		float getX() {return x;}
		float getY() {return y;}
		float getZ() {return z;}
		float getTileW() {return tileW;}
		float getTileH() {return tileH;}
		int getCenterLayer() {return centerLayer;}
		int getLayerCount() {return layerCount;}
		int getLayerSpacing() {return layerSpacing;}
		std::vector<SDL_Texture*> getTileTextures() {return tileTextures;}
		bool getEditMode() {return editMode;}

		void setX(float newX) {x = newX;}
		void setY(float newY) {y = newY;}
		void setZ(float newZ) {z = newZ;}
		void setCenterLayer(int newLayer) {centerLayer = newLayer;}
		void setLayerSpacing(int newSpace) {layerSpacing = newSpace;}
		void setEditMode(bool newBool) {editMode = newBool;}
		
	private:
		std::vector<SDL_Texture*> loadTileSheet(const char*, SDL_Renderer*);
		SDL_Texture* createTextureFromSurface(std::string, SDL_Surface*, SDL_Renderer* renderer);
		int convertStringToInt(std::string);
		float randomNumber(float, float);

		std::vector<TileMap*> layers;
		std::string mapFileName;
		std::vector<SDL_Texture*> tileTextures;
		float xOffset, yOffset;
		float x, y, z;
		float tileW, tileH;
		int centerLayer;
		int layerCount;
		int maxLayersDisplayed;
		int layerSpacing;
		int playerCloseness;
		bool editMode;

		tileLocation focusLocation;
};

#endif