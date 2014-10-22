#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class EventHandler
{
	public:
		EventHandler(Game&);
		~EventHandler();
		void handleEvent(SDL_Event);

	private:
		Game *gameRef;
};

#endif