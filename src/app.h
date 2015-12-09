#ifndef __APP_H__
#define __APP_H__

#include "core.h"
#include "timer.h"

/**
 * @brief Mouse buttons
 */
enum MOUSE_BUTTON {
	/**Left mouse button*/
	MOUSE_LEFT = SDL_BUTTON_LEFT,
	/**Middle mouse button*/
	MOUSE_MIDDLE = SDL_BUTTON_MIDDLE,
	/**Right mouse button*/
	MOUSE_RIGHT = SDL_BUTTON_RIGHT
};

/**
 * @class App
 * @brief Provides windows with OpenGL context
 */
class App {
	bool running;
	SDL_GLContext context;
	const Uint8 *keys;
	bool mouse_relative;
	unsigned int fps_time;
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
	 * @brief The time it took to draw the last frame
	 */
	unsigned int frame_time;
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
	 * @param gl_maj	The major GL version number to be used
	 * @param gl_min	The minor GL version number to be used
	 * @param flags		Additional flags to be used by SDL_CreateWindow.
				The default flag is SDL_WINDOW_OPENGL
	 */
	App(const char* title, int res_x, int res_y, int offset_x, int offset_y,
	    int gl_maj, int gl_min, unsigned int flags);
	~App();

	/**
	 * @brief Sets the window to grab the mouse
	 * When activate the mouse can not leave the window boundaries
	 * @param on	True to turn on, false to turn off
	 */
	void grab_mouse(bool on);
	/**
	 * @brief Activates the relative mouse motion mode
	 * In the relative mouse motion mode the cursor is invisible and stays
	 * in the middle of the screen. The x and y coordinates passed to the
	 * handle_mouse_motion function will be the relative horizontal and
	 * vertical distances traveled. 
	 * @param on	True to turn on, false to turn off
	 */
	void set_relative_mode(bool on);
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
	 *	  mouse motion. This function should be overriden
	 * @param x The x coordinate where the event occured or the relative mouse
		    movement along the x axis (see set_relative_mode)
	 * @param y The y coordinate where the event occured or the relative  mouse
		    movement along the y axis (see set_relative_mode)
	 */
	virtual void handle_mouse_motion(int x, int y);
	/**
	 * @brief This function is called by poll_events() to handle
	 *	  mouse wheel movements. This function should be overriden
	 * @param x The amount scrolled horizontally
	 * @param y The amount scrolled vertically
	 */
	virtual void handle_mouse_wheel(int x, int y);
	/**
	 * @brief This function is called by poll_events() to handle
	 *	  mouse button presses. This function should be overriden
	 * @param x The x coordinate where the event occured
	 * @param y The y coordinate where the event occured
	 * @param button The button that caused the event
	 * @param down True if a button was pressed, false if it was released
	 * @param clicks The number of clicks (e.g. doubleclick)
	 */
	virtual void handle_mouse_button(int x, int y, MOUSE_BUTTON button,
					 bool down, int clicks);
	/**
	 * @brief This function is called by run when the window is resized. 
	 * 	  This function should be overriden
	 */
	virtual void handle_resize();
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
