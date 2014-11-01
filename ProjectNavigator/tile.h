#ifndef TILE_H
#define TILE_H

class Tile
{
	public:
		Tile();
		~Tile();

		enum TileType {None, Grass};
		SDL_Rect getRect();

		//set functions
		void setCollision(bool boolIn) {collision = boolIn;}
		void setType(TileType newType) {type = newType;}
		void setTileTexture(SDL_Texture* newTexture) {tileTexture = newTexture;}
		void setX(float newX) {x = newX;}
		void setY(float newY) {y = newY;}
		void setZ(float newZ) {z = newZ;}
		void setWidth(float newW) {width = newW;}
		void setHeight(float newH) {height = newH;}

		//get functions
		bool getCollision() {return collision;}
		TileType getType() {return type;}
		SDL_Texture* gettileTexture() {return tileTexture;}
		float getX() {return x;}
		float getY() {return y;}
		float getZ() {return z;}
		float getWidth() {return width;}
		float getHeight() {return height;}

	private:
		TileType type;
		bool collision;
		SDL_Texture* tileTexture;
		float x, y, z;
		float width, height;
};

#endif