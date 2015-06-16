#ifndef __APP_H__
#define __APP_H__

#include "core.h"
#include "timer.h"

/**
 * @class App
 * @brief Provides windows with OpenGL context
 */
class App {
	bool running;
	SDL_GLContext context;
	const Uint8 *keys;
	Uint32 mouse_buttons;
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
	 * @param flags		Additional flags to be used in SDL_CreateWindow.
				The default flag is SDL_WINDOW_OPENGL
	 */
	App(const char* title, int res_x, int res_y, int offset_x, int offset_y,
	    int gl_maj, int gl_min, unsigned int flags);
	~App();

	/**
	 * @brief Sets the window to grab the mouse
	 * @param on	True to turn on, false to turn off
	 */
	void grab_mouse(SDL_bool on);
	/**
	 * @brief Turns VSync on or off
	 * @param on	True turns VSync on, false turns it off
	 * @return	Returns false if VSync is not supported, true otherwise
	 */
	bool enable_vsync(bool on);
	/**
	 * @brief Polls all events and calls the handlers. Called by the run function
	 */
	void poll_events();
	/**
	 * @brief This function is called by poll_events() to handle
	 *	  keyboard input. This function should be overriden
	 */
	virtual void handle_keyboard();
	/**
	 * @brief Checks if the key is currently pressed
	 * @param key The key to check
	 * @return Retruns true if the key is pressed, false otherwise
	 */
	bool key_pressed(const char *key);
	/**
	 * @brief This function is called by poll_events() to handle
	 *	  mouse input. This function should be overriden
	 */
	virtual void handle_mouse(int x, int y);
	/**
	 * @brief Checks if the mouse button is currently pressed
	 * @param button The button to check
	 * @return Retruns true if the mouse button is pressed, false otherwise
	 */
	bool mousebutton_pressed(int button);
	/**
	 * @brief This function is called by run() to draw a frame.
	 * 	  This function should be overriden
	 */
	virtual void display();
	/**
	 * @brief This is the mainloop. It does not limit the framerate
	 */
	void run();
	/**
	 * @brief This is the mainloop. It calls poll_events() and display()
	 * @param fps	The frames per second limit.
	 *		Any number below 1 means no limit
	 */
	void run(int fps);
};

#endif
