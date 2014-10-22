// File: game.cpp
// Prog: Dylan Gallardo
// Purp: Game initializes everything that has to do with the game.
//		 It also is in charge of updating the main game loop.

#include <SDL.h>
#include <iostream>

#include "game.h"
#include "eventHandler.h"
#include "timer.h"

using namespace std;

Game::Game()
{
	SCREEN_WIDTH = 640;
	SCREEN_HEIGHT = 480;
}


int Game::run()
{
	// Init SDL
	if (initSDL() == false)
	{
		cout << "SDL failed to initialize" << endl;
		return -1;
	}

	// Initialize variables
	quit = false;
	float prevTime = 0;
	EventHandler eventHandler(*this);

	// MAIN LOOP
	while (quit == false)
	{
		float deltaTime = (SDL_GetTicks() - prevTime) / 1000.f;
		prevTime = SDL_GetTicks();

		// Handle events (should be in a different function)
		while (SDL_PollEvent(&evt) != 0)
		{
			eventHandler.handleEvent(evt);
		}

		// Render
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_RenderPresent(renderer);
	}

	close();
	return 0;
}


bool Game::initSDL()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("SDL Init failed. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create window
	window = SDL_CreateWindow("Navigator Game", 600, 40, 640, 480, SDL_WINDOW_SHOWN);
	if (window == false)
	{
		printf("SDL Window failed to init. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}

	// Create renderer //SDL_RENDERER_ACCELERATED
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer == false)
	{
		printf("SDL failed to init renderer. SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	return true;
}


void Game::close()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;
	
	SDL_Quit();
}