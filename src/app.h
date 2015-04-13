#ifndef __APP_H__
#define __APP_H__

#include "core.h"
#include "timer.h"

/**
 * @class App
 * @brief Provides a window with OpenGL context.
 */
class App {
	SDL_GLContext context;
public:
	/**
	 * @brief The width of the window surface.
	 */
	int width;
	/**
	 * @brief The height of the window surface.
	 */
	int height;
	/**
	 * @brief The window surface.
	 */
	SDL_Window *window;

	/**
	 * @param title		The window tile.
	 * @param resX		The horizontal window resolution.
	 * @param resY		The vertical window resolution.
	 * @param offsetX	The horizontal position offset of the window.
	 * @param offsetY	The vertical position offset of the window.
	 * @param flags		Additional flags to be used in SDL_CreateWindow. The default flag is SDL_WINDOW_OPENGL.
	 */
	App(const char* title, int resX, int resY, int offsetX, int offsetY, unsigned int flags);
	~App();

	/**
	 * @brief Polls all events and calls the handlers.
	 */
	int poll_events();
	/**
	 * @brief This function is called by poll_events() to handle keyboard input. Should be overriden.
	 */
	virtual void handle_keyboard(bool *keys);
	/**
	 * @brief This function is called by run() to draw a frame. Should be overriden.
	 */
	virtual void display();
	/**
	 * @brief Equivalent to calling run(0).
	 */
	void run();
	/**
	 * @brief This is the mainloop. It calls poll_events() and display().
	 * @param fps The frames per second limit. Any number below 1 means no limit.
	 */
	void run(int fps);
};

#endif
