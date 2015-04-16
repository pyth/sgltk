#ifndef __APP_H__
#define __APP_H__

#include "core.h"
#include "timer.h"

/**
 * @class App
 * @brief Provides windows with OpenGL context
 */
class App {
	SDL_GLContext context;
public:
	/**
	 * @brief The width of the window surface
	 */
	int width;
	/**
	 * @brief The height of the window surface
	 */
	int height;
	/**
	 * @brief The window surface
	 */
	SDL_Window *window;

	/**
	 * @param title		The window tile
	 * @param res_x		The x window resolution
	 * @param res_y		The y window resolution
	 * @param offset_x	The x position offset of the window
	 * @param offset_y	The y position offset of the window
	 * @param flags		Additional flags to be used in SDL_CreateWindow. The default flag is SDL_WINDOW_OPENGL
	 */
	App(const char* title, int res_x, int res_y, int offset_x, int offset_y, unsigned int flags);
	~App();

	/**
	 * @brief Polls all events and calls the handlers
	 */
	int poll_events();
	/**
	 * @brief This function is called by poll_events() to handle keyboard input.
	 *	This function should be overriden
	 * @param keys The array of keys currently pressed
	 */
	virtual void handle_keyboard(bool *keys);
	/**
	 * @brief This function is called by run() to draw a frame.
	 * 	This function should be overriden
	 */
	virtual void display();
	/**
	 * @brief This is the mainloop. It does not limit the framerate
	 */
	void run();
	/**
	 * @brief This is the mainloop. It calls poll_events() and display()
	 * @param fps The frames per second limit. Any number below 1 means no limit
	 */
	void run(int fps);
};

#endif
