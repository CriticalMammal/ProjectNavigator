// File: main.cpp
// Prog: Dylan Gallardo
// Purp: This initializes a new Game class, which actually handles everything
//		 else associated with the game. This is here to start the program in
//		 a way C++ likes.

#include <SDL.h>

#include "game.h"


int main(int argc, char *args[])
{
	Game game;
	int runInt = game.run();
	return runInt;
}