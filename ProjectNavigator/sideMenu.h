//template for any side menu you may want to create

#ifndef SIDEMENU_H
#define SIDEMENU_H

class SideMenu
{
	protected:
		float x, y, width, height;
		float goalX, xLerp;
		bool menuOpen;
		SDL_Rect menuRect;
		double menuOpenCoordX;
		double menuOpenCoordY;
		double menuClosedCoordX;
		double menuClosedCoordY;
		
		float lerp(float, float, float);

	public:
		SideMenu();
		~SideMenu();

		void draw(SDL_Renderer*);
		void update();

		//getter/setters
		SDL_Rect getMenuRect() {return menuRect;}
		bool getMenuOpen() {return menuOpen;}
		double getMenuOpenCoordX() {return menuOpenCoordX;}
		double getMenuOpenCoordY() {return menuOpenCoordY;}

		void setMenuOpen(bool boolIn) {menuOpen = boolIn;}
		void setX(float newX) {x = newX;}
		void setY(float newY) {y = newY;}
		void setWidth(float newW) {width = newW;}
		void setHeight(float newH) {height = newH;}
};

#endif
