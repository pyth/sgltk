#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include "app.h"

namespace sgltk {

/**
 * @class Gamepad
 * @brief Manages and handles gamepads
 * @note You should never have to create or delete objects of this class
 * 	directly
 */
class Gamepad {
	SDL_GameController *gamepad;
	SDL_Joystick *joystick;
	SDL_Haptic *haptic;

	static unsigned int id_max;
	EXPORT static std::list<unsigned int> ids;

	public:
	/**
	 * @brief The gamepad id. When a new gamepad is attached it is assigned the
	 *	first free ID starting at 0.
	 */
	unsigned int id;
	/**
	 * @brief The instance ID of the gamepad as provided
	 *	by an SDL2 event
	 */
	unsigned int instance_id;
	/**
	* @brief The number of axes the gamepad has.
	*/
	unsigned int num_axes;
	/**
	* @brief The number of buttons the gamepad has.
	*/
	unsigned int num_buttons;
	/**
	* @brief The axis deadzone
	*/
	unsigned int deadzone;
	/**
	 * @brief A list of all buttons currently pressed
	 */
	std::vector<int> buttons_pressed;

	/**
	 * @param device_id The device id as provided by the SDL_CONTROLLERDEVICEADDED event
	 */
	EXPORT Gamepad(unsigned int device_id);
	EXPORT ~Gamepad();

	/**
	 * @brief Sets the state of a button
	 * @param button The button the state of which is to be set
	 * @param state The new state of the button
	 */
	EXPORT void set_button_state(int button, bool state);
	/**
	* @brief Sets a dedzone for all axes
	* @param deadzone The deadzone
	*/
	EXPORT void set_deadzone(unsigned int deadzone);
	/**
	 * @brief Call this function to get the current value of an axis
	 * @param axis The axis the value of which to get
	 * @return The current value of the axis in the range from 0 to 32767
	 *	for a trigger and -32768 to 32767 for an analog stick
	 */
	EXPORT int get_axis_value(unsigned int axis);
	/**
	 * @brief Plays a rumble effect
	 * @param magnitude The strength of the rumble on a scale from 0 to 1
	 * @param duration The duration of the effect in milliseconds
	 */
	EXPORT void play_rumble(float magnitude, unsigned int duration);
	/**
	 * @brief Stops the rumble effect
	 */
	EXPORT void stop_rumble();
};

}

#endif //__GAMEPAD_H__
