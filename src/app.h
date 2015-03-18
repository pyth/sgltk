#ifndef __APP_H__
#define __APP_H__

#include "core.h"
#include "timer.h"

class App {
	SDL_GLContext context;
public:
	int width;
	int height;
	SDL_Window *window;

	App(const char* title, int resX, int resY, int offsetX, int offsetY, unsigned int flags);
	~App();

	int poll_events();
	void handle_keyboard(bool *keys);
	virtual void display();
	void run();
	void run(int fps);
};

#endif
