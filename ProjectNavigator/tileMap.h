#ifndef TILEMAP_H
#define TILEMAP_H

class TileMap
{
	public:
		TileMap();
		~TileMap();

		void updateTiles();
		void drawTileMap(SDL_Rect, SDL_Renderer* renderer);

		void saveMapFile();
		bool loadMap(std::vector<std::string>);
		bool generateMap(int rowAmt, int columnAmt, double tileWidth, double tileHeight, std::vector<SDL_Texture*>);
		void replaceTile(Tile* newTile, int tileRow, int tileColumn);
		void setTileEmpty(int tileRow, int tileColumn);

		double getX() {return x;}
		double getY() {return y;}
		double getZ() {return z;}
		double getTileW() {return tileW;}
		double getTileH() {return tileH;}
		int getRowCount() {return rows;}
		int getColumnCount() {return columns;}

		void setX(double newX) {x = newX;}
		void setY(double newY) {y = newY;}
		void setZ(double newZ) {z = newZ;}
		void setDistFromCenter(int newDist) {distFromCenter = newDist;}

	private:
		std::vector< std::vector<Tile*> > tiles;
		std::vector<std::string> mapData;
		int rows, columns;
		int distFromCenter;
		double x, y, z, tileH, tileW;
		
		int convertStringToInt(std::string);
		float randomNumber(float, float);
};

#endif