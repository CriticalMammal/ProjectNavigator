#include <SDL.h>
#include <vector>

#include "sideMenu.h"
#include "button.h"
#include "tileEditorMenu.h"

extern int mouseX, mouseY;


TileEditorMenu::TileEditorMenu()
{
	buttonImagesSet = false;
	buttonAmt = 21;
	int menuPadHeight = 15;
	int menuPadWidth = 15;

	int buttonWidth = 40;
	int buttonHeight = 40;

	int buttonPad = 7;
	int buttonColumnWidth = 3;
	int buttonRow = 0;

	//create a loop to assign button positions
	while (buttons.size() < buttonAmt)
	{
		int buttonX = menuPadWidth;

		for (int w = 0; w<buttonColumnWidth; w++)
		{
			Button tempButton;
			buttons.push_back(tempButton);

			buttons.back().setX(buttonX);
			buttons.back().setY(menuPadHeight + (buttonRow*buttonHeight) + (buttonRow*buttonPad));
			buttons.back().setWidth(buttonWidth);
			buttons.back().setHeight(buttonHeight);

			buttonX += buttonWidth+buttonPad;
		}
		
		buttonRow ++;
	}

	menuRect.x = x;
	menuRect.y = y;
	menuRect.w = width;
	menuRect.h = height;

} //END TileEditorMenu()


TileEditorMenu::~TileEditorMenu()
{

}


void TileEditorMenu::updateTileEditor()
{
	//if button images need to be updated
	if (!buttonImagesSet)
	{
		for (int i=0; i<buttonAmt; i++)
		{
			if (i < tileImages.size())
			{
				buttons[i].setImage(tileImages[i]);
			}
			else
			{
				buttons[i].setImage(tileImages[0]);
			}
		}
		buttonImagesSet = true;
	}


	//normal button updates
	for (int i=0; i<buttonAmt; i++)
	{
		buttons[i].setMenuXOffset(x);
		buttons[i].setMenuYOffset(y);
		buttons[i].update();

		if (buttons[i].getButtonClicked())
		{
			clickedButtonType = i;
		}
	}

	menuRect.x = x;
	menuRect.y = y;
	menuRect.w = width;
	menuRect.h = height;

	//check to see if the mouse should be focused on the menu
	if (pointCollisionCheck(mouseX, mouseY))
	{
		menuMouseFocus = true;
	}
	else
	{
		menuMouseFocus = false;
	}

} //END updateTileEditor()


void TileEditorMenu::drawButtons(SDL_Renderer* renderer)
{
	for (int i=0; i<buttonAmt; i++)
	{
		buttons[i].draw(renderer);
	}
}


bool TileEditorMenu::pointCollisionCheck(double xVal, double  yVal)
{
	if (xVal >= menuRect.x && xVal <= menuRect.x + menuRect.w &&
		yVal >= menuRect.y && yVal <= menuRect.y + menuRect.h)
	{
		return true;
	}
	else
		return false;
}