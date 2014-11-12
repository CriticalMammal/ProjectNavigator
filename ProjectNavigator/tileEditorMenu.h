//inherets from SideMenu class. A specific side menu for editing the tile map in the game

#ifndef TILEEDITORMENU_H
#define TILEEDITORMENU_H

class TileEditorMenu : public SideMenu
{
	private:
		bool buttonImagesSet, menuMouseFocus;
		int buttonAmt, clickedButtonType;
		std::vector<Button> buttons;
		std::vector<SDL_Texture*> tileImages;

		bool pointCollisionCheck(double x, double y);


	public:
		TileEditorMenu();
		~TileEditorMenu();

		void updateTileEditor();
		void drawButtons(SDL_Renderer*);

		void setTileImages(std::vector<SDL_Texture*> images) {tileImages = images;}

		bool getMenuMouseFocus() {return menuMouseFocus;}
		int getClickedButtonType() {return clickedButtonType;}
};

#endif