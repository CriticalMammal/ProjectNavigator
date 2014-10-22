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


void EventHandler::handleEvent(SDL_Event eventIn)
{
	switch(eventIn.type)
	{
		case SDL_QUIT:
			gameRef->setQuit(true);
			break;
		case SDL_KEYDOWN:
			switch(eventIn.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					gameRef->setQuit(true);
					break;
			}
			break;
		case SDL_KEYUP:
			switch(eventIn.key.keysym.sym)
			{
				// whatever keys are released
			}
			break;
	}
}