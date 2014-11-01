#ifndef TILEMAP_H
#define TILEMAP_H

class TileMap
{
	public:
		TileMap();
		~TileMap();
		bool moveUp, moveDown, moveLeft, moveRight;

		void initialize(std::string fileLocation, int rowAmt, int columnAmt, double tileWidth, double tileHeight, SDL_Renderer*);
		void updateTiles();
		void drawTileMap(SDL_Rect, SDL_Renderer* renderer);

		void saveMapFile();
		bool loadMapFile(std::string);

		double getX() {return x;}
		double getY() {return y;}
		double getTileW() {return tileW;}
		double getTileH() {return tileH;}
		int getCenterRow() {return centerRow;}

		void setX(int newX) {x = newX;}
		void setY(int newY) {y = newY;}
		void setCenterRow(int newRow) {centerRow = newRow;}

	private:
		SDL_Surface *tileSheet;
		std::vector< std::vector<Tile*> > tiles;
		std::string mapFileName;
		int rows, columns;
		int centerRow, maxRowsDisplayed;
		double x, y, tileH, tileW;
		
		std::vector<SDL_Texture*> loadTileSheet(const char*, SDL_Renderer*);
		SDL_Texture* loadTexture(std::string, SDL_Surface*, SDL_Renderer* renderer);
		int convertStringToInt(std::string);
		float randomNumber(float, float);
};

#endif