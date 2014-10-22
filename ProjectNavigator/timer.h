#ifndef TIMER_H
#define TIMER_H

class Timer
{
	public:
		Timer();

		void start();
		void stop();
		void pause();
		void unpause();

		Uint32 getTicks();

		bool isStarted();
		bool isPaused();

	private:
		bool paused;
		bool started;

		Uint32 startTicks;
		Uint32 pausedTicks;
};

#endif