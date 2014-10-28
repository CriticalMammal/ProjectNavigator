#ifndef TILE_H
#define TILE_H

class Tile
{
	public:
		Tile();
		~Tile();

		enum TileType {None, Grass};

		//set functions
		void setCollision(bool boolIn) {collision = boolIn;}
		void setType(TileType newType) {type = newType;}
		void settileTexture(SDL_Texture *newTexture) {tileTexture = newTexture;}
		void setX(float newX) {x = newX;}
		void setY(float newY) {y = newY;}
		void setZ(float newZ) {z = newZ;}


		//get functions
		bool getCollision() {return collision;}
		TileType getType() {return type;}
		SDL_Texture *gettileTexture() {return tileTexture;}
		float getX() {return x;}
		float getY() {return y;}
		float getZ() {return z;}

	private:
		TileType type;
		bool collision;
		SDL_Texture *tileTexture;
		float x, y, z;
};

#endif