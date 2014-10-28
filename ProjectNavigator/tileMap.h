#ifndef TILEMAP_H
#define TILEMAP_H


class TileMap
{
	private:
		SDL_Surface *tileSheet;
		std::vector< std::vector<Tile>> tiles;
		std::string mapFileName;
		int rows, columns;
		double x, y, tileH, tileW;

		SDL_Texture* loadTexture(std::string, SDL_Surface*, SDL_Renderer* renderer);
		int convertStringToInt(string);
		float randomNumber(float, float);

	public:
		TileMap();
		~TileMap();

		void initialize(std::string fileLocation, int rowAmt, int columnAmt, double tileHeight, double tileWidth);
		void drawTileMap(SDL_Rect, SDL_Renderer* renderer);

		void saveMapFile();
		bool loadMapFile(string);

		double getTileW() {return tileW;}
		double getTileH() {return tileH;}

		void setX(int newX) {x = newX;}
		void setY(int newY) {y = newY;}
};

#endif