// File: game.cpp
// Prog: Dylan Gallardo
// Purp: Game initializes everything that has to do with the game.
//		 It also is in charge of updating the main game loop.

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>

#include "timer.h"
#include "tile.h"
#include "tileMap.h"
#include "game.h"
#include "eventHandler.h"

using namespace std;


Game::Game()
{
	screenWidth = 640;
	screenHeight = 480;
	quit = false;
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
	float prevTime = 0;
	EventHandler eventHandler(*this);
	theMap.initialize("null", 1000, 50, 50, 50, renderer);

	// Load Audio
	pageFlip = NULL;
	pageFlip = Mix_LoadWAV("audio/pageFlip.wav");
	if (pageFlip == NULL)
	{
		cout << "Unable to load a sound. Mixer_ERROR: " << Mix_GetError() << endl;
	}
	Mix_VolumeChunk(pageFlip, 20);
	channel = Mix_PlayChannel(-1, pageFlip, 0);

	// MAIN LOOP
	while (quit == false)
	{
		// set deltaTime (time between frames)
		float deltaTime = (SDL_GetTicks() - prevTime) / 1000.f;
		prevTime = SDL_GetTicks();

		// Handle events
		eventHandler.handleEvents();

		// Handle game logic
		SDL_Rect screenRect = {0, 0, screenWidth, screenHeight};
		theMap.updateTiles();
		if (theMap.moveDown || theMap.moveUp)
		{
			//channel = Mix_PlayChannel(-1, pageFlip, 0);
		}

		// Render
		SDL_SetRenderDrawColor(renderer, 88, 232, 206, 0);
		SDL_RenderClear(renderer);

		theMap.drawTileMap(screenRect, renderer);

		SDL_RenderPresent(renderer);
	}

	Mix_FadeOutChannel(channel, 1000);
	Mix_FreeChunk(pageFlip);
	close();
	return 0;
} // END run()


void Game::handleKey(SDL_Event event)
{
	switch (event.type)
	{
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_UP:
					theMap.moveUp = true;
					break;
				case SDLK_DOWN:
					theMap.moveDown = true;
					break;
				case SDLK_RIGHT:
					theMap.moveRight = true;
					break;
				case SDLK_LEFT:
					theMap.moveLeft = true;
					break;
			}
			break;
		case SDL_KEYUP:
			switch(event.key.keysym.sym)
			{
				case SDLK_UP:
					theMap.moveUp = false;
					break;
				case SDLK_DOWN:
					theMap.moveDown = false;
					break;
				case SDLK_RIGHT:
					theMap.moveRight = false;
					break;
				case SDLK_LEFT:
					theMap.moveLeft = false;
					break;
			}
			break;
	}
}


void Game::playPageFlip()
{
	channel = Mix_PlayChannel(-1, pageFlip, 0);
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
	window = SDL_CreateWindow("Navigator Game", 600, 40, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
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

	// Init Audio Mixer
	int audio_rate = 22050;
	uint16_t audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		cout << "Audio failed to initialize. SDL_ERROR: " << Mix_GetError() << endl;
		return false;
	}
	Mix_AllocateChannels(20);

	return true;
}


void Game::close()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;

	Mix_CloseAudio();
	
	SDL_Quit();
}