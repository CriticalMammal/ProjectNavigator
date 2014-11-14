// File: game.h
// Prog: Dylan Gallardo
// Purp: header file for game.cpp

#ifndef GAME_H
#define GAME_H

// I DO NOT LIKE HAVING TO DO THIS
// JUST TO DECLARE THE TILEMAP HERE
#include <SDL_mixer.h>
#include <vector>
#include "tile.h"
#include "tileMap.h"
#include "gameMap.h"
#include "button.h"
#include "sideMenu.h"
#include "tileEditorMenu.h"

class Game
{
	public:
		Game();
		//~Game();
		int run();
		void handleKey(SDL_Event);
		void handleMouse(SDL_Event);
		void playPageFlip();

		void setQuit(bool boolIn) {quit = boolIn;}
		bool getQuit() {return quit;}

	private:
		bool initSDL();
		void close();

		SDL_Window *window;
		SDL_Renderer *renderer;
		GameMap theMap;
		TileEditorMenu tileEditorMenu;
		int screenWidth;
		int screenHeight;
		bool quit;
		bool editMode;
		Mix_Chunk *pageFlip;
		Mix_Chunk *bgMusic;
		int channel;
};

#endif