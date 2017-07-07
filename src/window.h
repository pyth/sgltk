#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "app.h"
#include "image.h"
#include "timer.h"
#include "gamepad.h"
#include "joystick.h"

namespace sgltk {

/**
 * @brief Window fullscreen states
 */
enum FULLSCREEN_MODE {
	/**
	 * @brief Windowed mode
	 */
	WINDOWED = 0,
	/**
	 * @brief Real Fullscrren mode
	 */
	FULLSCREEN = SDL_WINDOW_FULLSCREEN,
	/**
	 * @brief Fake fullscreen mode
	 */
	FAKE_FULLSCREEN = SDL_WINDOW_FULLSCREEN_DESKTOP
};

/**
 * @class Window
 * @brief Provides windows with OpenGL context
 */
class Window {
	bool running;
	SDL_GLContext context;
	const Uint8 *keys;
	bool mouse_relative;
	unsigned int fps_time;
	std::vector<std::string> keys_pressed;
	EXPORT static std::map<unsigned int, Gamepad *> gamepad_instance_id_map;
	EXPORT static std::map<unsigned int, Joystick *> joystick_instance_id_map;
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
	double delta_time;
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
	 * @param flags		Additional flags to be used by SDL_CreateWindow.
				The default flags are SDL_WINDOW_OPENGL and SDL_WINDOW_RESIZABLE
	 */
	EXPORT Window(const std::string& title, int res_x, int res_y, int offset_x, int offset_y, unsigned int flags = 0);
	EXPORT ~Window();

	/**
	* @brief Sets the window icon
	* @param filename The icon to use
	* @note The image size should not exceed 356x356 pixel.
	*/
	EXPORT void set_icon(const std::string& filename);
	/**
	 * @brief Sets the window icon
	 * @param icon The icon to use
	 * @note The image size should not exceed 356x356 pixel.
	 */
	EXPORT void set_icon(const sgltk::Image& icon);
	/**
	 * @brief Set window title
	 * @param title The new window title
	 */
	EXPORT void set_title(const std::string& title);
	/**
	 * @brief Sets or removes the ability to resize the window
	 * @param on If true, the window is made resizable. If false, window becomes non-resizable.
	 */
	EXPORT void set_resizable(bool on);
	/**
	 * @brief Takes a screenshot of the window
	 * @param image The image to save the screenshot to
	 */
	EXPORT void take_screenshot(sgltk::Image& image);
	/**
	 * @brief Sets the window to grab the mouse
	 * When activate the mouse can not leave the window boundaries
	 * @param on	True to turn on, false to turn off
	 */
	EXPORT void grab_mouse(bool on);
	/**
	 * @brief Returns the index of the display that currently contains the
	 * 	window. This index corresponds to the indices in the
	 * 	sgltk::App::sys_info.desktop_display_modes and
	 * 	sgltk::app::sys_info.supported_display_modes lists
	 * @return Returns the index of the display that currently contains the
	 * 	window or a negative number of failure.
	 */
	EXPORT int get_display_index();
	/**
	 * @brief Changes the window display mode when in fullscreen mode
	 * @param mode The display mode to set
	 * @return Returns true on success, false otherwise
	 */
	EXPORT bool set_display_mode(const SDL_DisplayMode& mode);
	/**
	 * @brief Changes the window fullscreen mode
	 * @param mode The fullscreen mode to set
	 * @return Returns true on success or false on failure
	 */
	EXPORT bool fullscreen_mode(FULLSCREEN_MODE mode);
	/**
	 * @brief Activates the relative mouse motion mode
	 * In the relative mouse motion mode the cursor is invisible and stays
	 * in the middle of the screen. The x and y coordinates passed to the
	 * handle_mouse_motion function will be the relative horizontal and
	 * vertical distances traveled.
	 * @param on	True to turn on, false to turn off
	 */
	EXPORT void set_relative_mode(bool on);
	/**
	 * @brief Polls all events and calls the handlers.
	 * 	  Called by the run function
	 */
	EXPORT void poll_events();
	/**
	 * @brief This function is called by poll_events() to handle
	 *	  the addition of new gamepads. This function should be
	 *	  overridden
	 * @param gamepad_id The gamepad instance id
	 */
	EXPORT virtual void handle_gamepad_added(unsigned int gamepad_id);
	/**
	 * @brief This function is called by poll_events() to handle
	 *	  the removal of gamepads. This function should be overridden
	 * @param gamepad_id The gamepad instance id
	 */
	EXPORT virtual void handle_gamepad_removed(unsigned int gamepad_id);
	/**
	 * @brief This function is called by poll_events() every frame for every
	 * 	  gamepad button currently being pressed. This function should
	 * 	  be overridden
	 * @param gamepad_id The gamepad instance id
	 * @param button The number of the button pressed or released
	 */
	EXPORT virtual void handle_gamepad_button(unsigned int gamepad_id,
								int button);
	/**
	 * @brief This function is called by poll_events() once for every
	 * 	  gamepad button press or release. This function should
	 * 	  be overridden
	 * @param gamepad_id The gamepad instance id
	 * @param button The number of the button pressed or released
	 * @param pressed Indicates whether the button was pressed (true) or
	 * 		  released (false)
	 */
	EXPORT virtual void handle_gamepad_button_press(unsigned int gamepad_id,
							int button,
							bool pressed);
	/**
	 * @brief This function is called by poll_events() every frame for
	 * 	  every axis of every gamepad. This function should be overridden
	 * @param gamepad_id The gamepad instance id
	 * @param axis The number of the axis that has a new value
	 * @param value The new value of the axis
	 */
	EXPORT virtual void handle_gamepad_axis(unsigned int gamepad_id,
						unsigned int axis, int value);
	/**
	 * @brief This function is called by poll_events() for every axis value
	 * 	  change. This function should be overridden
	 * @param gamepad_id The gamepad instance id
	 * @param axis The number of the axis that has a new value
	 * @param value The new value of the axis
	 */
	EXPORT virtual void handle_gamepad_axis_change(unsigned int gamepad_id,
							unsigned int axis,
							int value);
	/**
	* @brief This function is called by poll_events() to handle
	*	  the addition of new joysticks. This function should be
	*	  overridden
	* @param gamepad_id The joystick instance id
	*/
	EXPORT virtual void handle_joystick_added(unsigned int gamepad_id);
	/**
	* @brief This function is called by poll_events() to handle
	*	  the removal of joysticks. This function should be overridden
	* @param gamepad_id The joystick instance id
	*/
	EXPORT virtual void handle_joystick_removed(unsigned int gamepad_id);
	/**
	* @brief This function is called by poll_events() every frame for every
	* 	  joystick button currently being pressed. This function should
	* 	  be overridden
	* @param gamepad_id The joystick instance id
	* @param button The number of the button pressed or released
	*/
	EXPORT virtual void handle_joystick_button(unsigned int gamepad_id,
		int button);
	/**
	* @brief This function is called by poll_events() once for every
	* 	  joystick button pressed or released. This function should
	* 	  be overridden
	* @param gamepad_id The joystick instance id
	* @param button The number of the button pressed or released
	* @param pressed Indicates whether the button was pressed (true) or
	* 		  released (false)
	*/
	EXPORT virtual void handle_joystick_button_press(unsigned int gamepad_id,
		int button,
		bool pressed);
	/**
	* @brief This function is called by poll_events() every frame for
	* 	  every axis of every joystick. This function should be overridden
	* @param gamepad_id The joystick instance id
	* @param axis The number of the axis that has a new value
	* @param value The new value of the axis
	*/
	EXPORT virtual void handle_joystick_axis(unsigned int gamepad_id,
		unsigned int axis, int value);
	/**
	* @brief This function is called by poll_events() for every axis value
	* 	  change. This function should be overridden
	* @param gamepad_id The gamepad instance id
	* @param axis The number of the axis that has a new value
	* @param value The new value of the axis
	*/
	EXPORT virtual void handle_joystick_axis_change(unsigned int gamepad_id,
		unsigned int axis,
		int value);
	/**
	* @brief This function is called by poll_events() every frame for
	* 	  every hat of every joystick. This function should be overridden
	* @param gamepad_id The joystick instance id
	* @param hat The number of the hat that has a new value
	* @param value The new value of the hat
	*/
	EXPORT virtual void handle_joystick_hat(unsigned int gamepad_id,
		unsigned int hat, unsigned int value);
	/**
	* @brief This function is called by poll_events() for every hat value
	* 	  change. This function should be overridden
	* @param gamepad_id The gamepad instance id
	* @param hat The number of the hat that has a new value
	* @param value The new value of the hat
	*/
	EXPORT virtual void handle_joystick_hat_change(unsigned int gamepad_id,
		unsigned int hat,
		unsigned int value);
	/**
	* @brief This function is called by poll_events() for every ball
	*	 that has changed value change.
	*	 This function should be overridden
	* @param gamepad_id The gamepad instance id
	* @param ball The number of the hat that has a new value
	* @param xrel The new value of the hat
	* @param yrel The new value of the hat
	*/
	EXPORT virtual void handle_joystick_ball_motion(unsigned int gamepad_id,
		unsigned int ball,
		int xrel, int yrel);
	/**
	 * @brief This function is called by poll_events() every frame for every
	 * 	  key currently being pressed. This function should be
	 * 	  overridden
	 * @param key The name of the key being pressed
	 */
	EXPORT virtual void handle_keyboard(const std::string& key);
	/**
	 * @brief This function is called by poll_events() once for every key
	 * 	pressed or released. This function should be overridden
	 * @param key The name of the key being pressed
	 * @param pressed True if the key has been pressed, false otherwise
	 */
	EXPORT virtual void handle_key_press(const std::string& key, bool pressed);
	/**
	 * @brief This function is called by poll_events() to handle
	 *	  mouse motion. This function should be overridden
	 * @param x The x coordinate where the event occurred or the relative
	 * 	    mouse movement along the x axis (see set_relative_mode)
	 * @param y The y coordinate where the event occurred or the relative
	 * 	    mouse movement along the y axis (see set_relative_mode)
	 */
	EXPORT virtual void handle_mouse_motion(int x, int y);
	/**
	 * @brief This function is called by poll_events() to handle
	 *	  mouse wheel movements. This function should be overridden
	 * @param x The amount scrolled horizontally
	 * @param y The amount scrolled vertically
	 */
	EXPORT virtual void handle_mouse_wheel(int x, int y);
	/**
	 * @brief This function is called by poll_events() to handle
	 *	  mouse button presses. This function should be overridden
	 * @param x The x coordinate where the event occurred
	 * @param y The y coordinate where the event occurred
	 * @param button The button that caused the event
	 * @param down True if a button was pressed, false if it was released
	 * @param clicks The number of clicks (e.g. double click)
	 */
	EXPORT virtual void handle_mouse_button(int x, int y, int button,
					 bool down, int clicks);
	/**
	 * @brief This function is called by run when the window is resized.
	 * 	  This function should be overridden
	 */
	EXPORT virtual void handle_resize();
	/**
	 * @brief This function is called by run when the window is being
	 * 	  closed. This function should be overridden
	 */
	EXPORT virtual void handle_exit();
	/**
	 * @brief This function is called by run() to draw a frame.
	 * 	  This function should be overridden
	 */
	EXPORT virtual void display();
	/**
	 * @brief Starts the main loop. This function calls poll_events() and
	 * 	  display()
	 * @param fps	The frames per second limit.
	 *		Any number below 1 means no limit
	 */
	EXPORT void run(unsigned int fps = 0);
	/**
	 * @brief Stops the main loop
	 */
	EXPORT void stop();
};

}

#endif
