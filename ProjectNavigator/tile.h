#ifndef TILE_H
#define TILE_H

class Tile
{
	public:
		Tile();
		~Tile();

		enum TileType 
		{
			dirt0 = 0,    dirt1 = 1,    dirt2 = 2, 
			wall0 = 3,    wall1 = 4,    wall2 = 5, 
			window0 = 6,  window1 = 7,  window2 = 8, 
			roof0 = 9,    roof1 = 10,   roof2 = 11, 
			pillar0 = 12, pillar1 = 13, pillar2 = 14, 
			arch0 = 15,   arch1 = 16,   arch2 = 17, 
			cactus0 = 18, cactus1 = 19, cactus2 = 20, 
			none = 21, 
			player = 22
		};
		void updateTile();
		SDL_Rect getRect();
		bool canMove();
		bool canFall();
		bool canJump();
		virtual void draw(SDL_Renderer* renderer);

		//set functions
		void setCollision(bool boolIn) {collision = boolIn;}
		void setType(TileType newType) {type = newType;}
		void setTileTexture(SDL_Texture* newTexture) {tileTexture = newTexture;}
		void setX(float newX) {goalX = newX;}
		void setY(float newY) {goalY = newY;}
		void setZ(float newZ) {goalZ = newZ;}
		void setWidth(float newW) {width = newW;}
		void setHeight(float newH) {height = newH;}
		void setEmpty(bool boolIn) {empty = boolIn;}
		void setIsEditTile(bool boolIn) {isEditTile = boolIn;}

		//get functions
		bool getCollision() {return collision;}
		TileType getType() {return type;}
		SDL_Texture* gettileTexture() {return tileTexture;}
		float getX() {return goalX;}
		float getY() {return goalY;}
		float getZ() {return goalZ;}
		float getWidth() {return width;}
		float getHeight() {return height;}
		bool getEmpty() {return empty;}
		bool getIsEditTile() {return isEditTile;}

	protected:
		TileType type;
		bool collision;
		SDL_Texture* tileTexture;
		float x, y, z;
		float goalX, goalY, goalZ;
		float xLerp, yLerp, zLerp;
		float width, height;
		float movementWaitTime, prevMovementTime;
		float airWaitTime, prevAirTime;
		float jumpWaitTime, prevJumpTime;
		bool empty, isEditTile;

		virtual void additionalAnimation();
		float lerp(float, float, float);
};

#endif