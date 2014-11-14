// This is currently not useable, sadly... I'll need to 
// figure out how to create a class like this later

#include <SDL.h>
#include <iostream>
#include <functional>

#include "game.h"
#include "eventHandler.h"

using namespace std;


EventHandler::EventHandler(Game& gameRefIn)
{
	gameRef = &gameRefIn;
}


EventHandler::~EventHandler()
{
	gameRef = nullptr;
}


void EventHandler::handleEvents()
{
	while (SDL_PollEvent(&evt) != 0)
	{
		switch(evt.type)
		{
			case SDL_QUIT:
				gameRef->setQuit(true);
				break;
			case SDL_KEYDOWN:
				gameRef->handleKey(evt);
				break;
			case SDL_KEYUP:
				gameRef->handleKey(evt);
				break;
			case SDL_MOUSEBUTTONDOWN:
				gameRef->handleMouse(evt);
				break;
			case SDL_MOUSEBUTTONUP:
				gameRef->handleMouse(evt);
				break;
		}
	}
}