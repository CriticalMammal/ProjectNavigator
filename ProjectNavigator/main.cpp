// File: main.cpp
// Prog: Dylan Gallardo
// Purp: This initializes a new Game class, which actually handles everything
//		 else associated with the game. This is literally just to kick
//		 the program off.

#include <SDL.h>
#include <SDL_mixer.h>

#include "game.h"


int main(int argc, char *args[])
{
	Game game;
	int runInt = game.run();
	return runInt;
}