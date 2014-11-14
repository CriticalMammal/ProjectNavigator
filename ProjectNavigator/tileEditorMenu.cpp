#include <SDL.h>
#include <vector>

#include "sideMenu.h"
#include "button.h"
#include "tileEditorMenu.h"

using namespace std;
extern int mouseX, mouseY;


TileEditorMenu::TileEditorMenu()
{
	buttonImagesSet = false;
	buttonClicked = false;
	menuPages = 2;
	currentPage = 0;
	buttonAmt = 21;
	menuPadHeight = 15;
	menuPadWidth = 15;

	buttonWidth = 40;
	buttonHeight = 40;

	buttonPad = 7;
	buttonColumnWidth = 3;
	int buttonRow = 0;

	//create a loop to assign button positions
	for (int r = 0; r < menuPages; r++)
	{
		vector<Button> buttonPage;

		while(buttonPage.size() < buttonAmt)
		{
			int buttonX = menuPadWidth;

			for (int w = 0; w<buttonColumnWidth; w++)
			{
				Button tempButton;

				tempButton.setX(buttonX);
				tempButton.setY(menuPadHeight + (buttonRow*buttonHeight) + (buttonRow*buttonPad));
				tempButton.setWidth(buttonWidth);
				tempButton.setHeight(buttonHeight);

				buttonPage.push_back(tempButton);

				buttonX += buttonWidth+buttonPad;
			}
		
			buttonRow ++;
		}

		buttons.push_back(buttonPage);
		buttonRow = 0; //reset row for next page
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
		int page = 0;

		while (page < menuPages)
		{
			for (int i=0; i<buttonAmt; i++)
			{
				if (i+(page*buttonAmt) < tileImages.size())
				{
					buttons[page][i].setImage(tileImages[i+(page*buttonAmt)]);
				}
				else
				{
					buttons[page][i].setImage(tileImages[0]);
				}
			}

			page ++;
		}

		buttonImagesSet = true;
	}


	//normal button updates
	for (int i=0; i<buttonAmt; i++)
	{
		buttons[currentPage][i].setMenuXOffset(x);
		buttons[currentPage][i].setMenuYOffset(y);
		buttons[currentPage][i].update();

		if (buttons[currentPage][i].getButtonClicked())
		{
			clickedButtonType = i + currentPage*buttonAmt;
			buttonClicked = true;
			break;
		}
		else
		{
			buttonClicked = false;
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


int TileEditorMenu::handleButtonClick()
{
	if (buttonClicked)
	{
		return clickedButtonType;
		buttonClicked = false;
	}
	else
	{
		return -1;
	}
}


void TileEditorMenu::drawButtons(SDL_Renderer* renderer)
{
	for (int i=0; i<buttonAmt; i++)
	{
		buttons[currentPage][i].draw(renderer);
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