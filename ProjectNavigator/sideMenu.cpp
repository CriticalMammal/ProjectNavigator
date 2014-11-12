#include <SDL.h>
#include <string>
#include <iostream>

#include "sideMenu.h"


SideMenu::SideMenu()
{
	menuOpen = false;

	width = 160;
	height = 350;
	x = 640+10;
	y = (480/2)-(height/2);
	xLerp = 0;

	menuClosedCoordX = 640+10;
	menuClosedCoordY = y;
	menuOpenCoordX = 640-width;
	menuOpenCoordY = y;

	menuRect.x = x;
	menuRect.y = y;
	menuRect.h = height;
	menuRect.w = width;
}


SideMenu::~SideMenu()
{

}


void SideMenu::update()
{
	if (menuOpen)
	{
		goalX = menuOpenCoordX;
	}
	else
	{
		goalX = menuClosedCoordX;
	}

	// Move to correct position
	if (x != goalX)
	{
		xLerp = 0.0;
	}

	if (xLerp < 1.0)
	{
		xLerp += 0.1;
	}

	float newX = lerp(x, xLerp, goalX);
	x = newX;

	//update rect
	menuRect.x = x;
	menuRect.y = y;
	menuRect.h = height;
	menuRect.w = width;
}


void SideMenu::draw(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
	SDL_RenderFillRect(renderer, &menuRect);
}


float SideMenu::lerp(float x, float t, float y)
{
	return x * (1-t) + y*t;
}