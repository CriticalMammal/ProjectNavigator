#ifndef PLAYERTILE_H
#define PLAYERTILE_H

class Player : public Tile
{
	public:
		Player();
		~Player();
		void draw(SDL_Renderer* renderer);

	private:
		void additionalAnimation();
		float randomNumber(float, float);
		float animationWaitTime, prevAnimationTime;

		std::vector<SDL_Rect> animatedBlockGoal;
		std::vector<SDL_Rect> animatedBlock;
};

#endif