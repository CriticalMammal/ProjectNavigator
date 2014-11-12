#ifndef BUTTON_H
#define BUTTON_H

class Button
{
	private:
		float x, y, width, height;
		SDL_Rect collisionRect;
		double menuXOffset, menuYOffset;
		bool mouseClicked, buttonIsHovered, buttonClicked;
		SDL_Texture* buttonTexture;

		bool pointCollisionCheck(double x, double y);

	public:
		Button();
		~Button();

		void init(SDL_Rect rectIn);
		void update();
		void draw(SDL_Renderer*);

		void setX(float newX) {x = newX;}
		void setY(float newY) {y = newY;}
		void setWidth(float newW) {width = newW;}
		void setHeight(float newH) {height = newH;}
		void setMouseClicked(bool boolIn) {mouseClicked = boolIn;}
		void setButtonClicked(bool boolIn) {buttonClicked = boolIn;}
		void setButtonHovered(bool boolIn) {buttonIsHovered = boolIn;}
		void setImage(SDL_Texture* texture) {buttonTexture = texture;}
		void setMenuXOffset(double xVal) {menuXOffset = xVal;}
		void setMenuYOffset(double yVal) {menuYOffset = yVal;}

		bool getButtonClicked() {return buttonClicked;}
};

#endif