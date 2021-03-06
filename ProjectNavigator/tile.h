#ifndef TILE_H
#define TILE_H

class Tile
{
	public:
		Tile();
		~Tile();

		enum TileType {None, Grass};
		void updateTile();
		SDL_Rect getRect();

		//set functions
		void setCollision(bool boolIn) {collision = boolIn;}
		void setType(TileType newType) {type = newType;}
		void setTileTexture(SDL_Texture* newTexture) {tileTexture = newTexture;}
		void setX(float newX) {goalX = newX;}
		void setY(float newY) {goalY = newY;}
		void setZ(float newZ) {goalZ = newZ;}
		void setWidth(float newW) {width = newW;}
		void setHeight(float newH) {height = newH;}

		//get functions
		bool getCollision() {return collision;}
		TileType getType() {return type;}
		SDL_Texture* gettileTexture() {return tileTexture;}
		float getX() {return goalX;}
		float getY() {return goalY;}
		float getZ() {return goalZ;}
		float getWidth() {return width;}
		float getHeight() {return height;}

	private:
		TileType type;
		bool collision;
		SDL_Texture* tileTexture;
		float x, y, z;
		float goalX, goalY, goalZ;
		float xLerp, yLerp, zLerp;
		float width, height;

		float lerp(float, float, float);
};

#endif