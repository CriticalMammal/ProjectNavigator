// File: game.h
// Prog: Dylan Gallardo
// Purp: header file for game.cpp

#ifndef GAME_H
#define GAME_H

class Game
{
	public:
		Game();
		//~Game();
		int run();

	private:
		bool initSDL();
		void close();

		SDL_Event evt;
		SDL_Window *window;
		SDL_Renderer *renderer;
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;
};

#endif