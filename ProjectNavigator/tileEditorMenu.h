//inherets from SideMenu class. A specific side menu for editing the tile map in the game

#ifndef TILEEDITORMENU_H
#define TILEEDITORMENU_H

class TileEditorMenu : public SideMenu
{
	private:
		bool buttonImagesSet, menuMouseFocus, buttonClicked;
		int menuPages, currentPage, buttonAmt, clickedButtonType;
		int menuPadHeight, menuPadWidth, buttonWidth, buttonHeight,
			buttonPad, buttonColumnWidth;
		std::vector< std::vector<Button> > buttons;
		std::vector<SDL_Texture*> tileImages;

		bool pointCollisionCheck(double x, double y);


	public:
		TileEditorMenu();
		~TileEditorMenu();

		void updateTileEditor();
		void drawButtons(SDL_Renderer*);
		int handleButtonClick();

		void setTileImages(std::vector<SDL_Texture*> images) {tileImages = images;}
		void setCurrentPage(int newPage) {currentPage = newPage;}

		bool getMenuMouseFocus() {return menuMouseFocus;}
		int getClickedButtonType() {return clickedButtonType;}
		int getCurrentPage() {return currentPage;}
		int getMaxPages() {return menuPages;}
};

#endif