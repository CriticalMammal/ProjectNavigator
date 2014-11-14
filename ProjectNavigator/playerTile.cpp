#include <SDL.h>
#include <vector>

#include "tile.h"
#include "playerTile.h"

Player::Player()
{
	prevAnimationTime = SDL_GetTicks();
	animationWaitTime = 0.1*1000; // In milliseconds
	movementWaitTime = 0.5*1000;

	int shiftingBlockCount = 10;

	for (int i=0; i < shiftingBlockCount; i++)
	{
		SDL_Rect newRect = {0, 0, 5, 5};
		animatedBlockGoal.push_back(newRect);
		animatedBlock.push_back(newRect);
	}
}

Player::~Player()
{

}


void Player::additionalAnimation()
{
	// Check if it's time to update
	if (SDL_GetTicks()-prevAnimationTime >= animationWaitTime)
	{
		for (int i=0; i<animatedBlockGoal.size(); i++)
		{
			animatedBlockGoal[i].x = randomNumber(x, x+width*z);
			animatedBlockGoal[i].y = randomNumber(y, y+height*z);
			animatedBlock[i].w = width*z/20;
			animatedBlock[i].h = height*z/20;
		}

		prevAnimationTime = SDL_GetTicks();
	}
	else
	{
		for (int i=0; i<animatedBlock.size(); i++)
		{
			// Lerp x
			if (animatedBlock[i].x != animatedBlockGoal[i].x)
			{
				xLerp = 0.0;
			}

			if (xLerp < 1.0)
			{
				xLerp += 0.1;
			}

			float newX = lerp(animatedBlock[i].x, xLerp, animatedBlockGoal[i].x);
			animatedBlock[i].x = newX;

			// Lerp y
			if (animatedBlock[i].y != animatedBlockGoal[i].y)
			{
				yLerp = 0.0;
			}

			if (yLerp < 1.0)
			{
				yLerp += 0.1;
			}

			float newY = lerp(animatedBlock[i].y, yLerp, animatedBlockGoal[i].y);
			animatedBlock[i].y = newY;
		}
	}
}


void Player::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	for (int i=0; i<animatedBlock.size(); i++)
	{
		SDL_RenderFillRect(renderer, &animatedBlock[i]);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);

	SDL_RenderFillRect(renderer, &getRect());
}


float Player::randomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}