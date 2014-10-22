#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class EventHandler
{
	public:
		EventHandler(Game&);
		~EventHandler();
		void handleEvents();

	private:
		Game *gameRef;
		SDL_Event evt;
};

#endif