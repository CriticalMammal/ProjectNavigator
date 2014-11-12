#include <SDL.h>

#include "button.h"

extern bool leftClick;
extern int mouseX, mouseY;


Button::Button()
{
	menuXOffset = 0;
	menuYOffset = 0;

	x = 0;
	y = 0;
	width = 50;
	height = 50;

	collisionRect.x = x;
	collisionRect.y = y;
	collisionRect.w = width;
	collisionRect.h = height;
}



Button::~Button()
{

}



//initialization of the button
void Button::init(SDL_Rect rectIn)
{
	collisionRect = rectIn;

	//eventually add in graphics for the button?

} //END init()



//update button
void Button::update()
{
	mouseClicked = leftClick;
	
	collisionRect.x = x+menuXOffset;
	collisionRect.y = y+menuYOffset;
	collisionRect.w = width;
	collisionRect.h = height;


	if (pointCollisionCheck(mouseX, mouseY))
	{
		if (mouseClicked)
		{
			buttonClicked = true;
		}
		else
		{
			buttonIsHovered = true;
			buttonClicked = false;
		}
	}
	else
	{
		buttonClicked = false;
		buttonIsHovered = false;
	}

} //END update()



void Button::draw(SDL_Renderer* renderer)
{
	//draw the tile image
	SDL_RenderCopy(renderer, buttonTexture, NULL, &collisionRect);


	//if mouse is just hovering or if clicked
	if (buttonClicked)
	{
		//draw a semi-transparent white square over the image
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
		SDL_RenderFillRect(renderer, &collisionRect);
	}
	else if (buttonIsHovered)
	{
		//draw a semi-transparent black square over the image
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
		SDL_RenderFillRect(renderer, &collisionRect);
	}
}


bool Button::pointCollisionCheck(double xVal, double  yVal)
{
	if (xVal >= collisionRect.x && xVal <= collisionRect.x + collisionRect.w &&
		yVal >= collisionRect.y && yVal <= collisionRect.y + collisionRect.h)
	{
		return true;
	}
	else
		return false;
}