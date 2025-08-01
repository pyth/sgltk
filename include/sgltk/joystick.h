#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "app.h"

namespace sgltk {

/**
* @class Joystick
* @brief Manages and handles joysticks
* @note You should never have to create or delete objects of this class
* 	directly
*/
class Joystick {
	SDL_Joystick *joystick;
	std::vector<int> switches;

	static unsigned int id_max;
	EXPORT static std::list<unsigned int> ids;

	public:
	/**
	 * @brief The joystick id. When a new joystick is attached it is assigned the
	 *	first free ID starting at 0.
	 */
	unsigned int id;
	/**
	* @brief The instance ID of the gamepad as provided
	*	by an SDL2 event
	*/
	unsigned int instance_id;
	/**
	 * @brief The name of the joystick
	 */
	std::string name;
	/**
	 * @brief The number of axes the joystick has.
	 */
	unsigned int num_axes;
	/**
	 * @brief The number of buttons the joystick has.
	 */
	unsigned int num_buttons;
	/**
	 * @brief The number of hats the joystick has.
	 */
	unsigned int num_hats;
	/**
	 * @brief The axis deadzone
	 */
	unsigned int deadzone;
	/**
	* @brief A list of all buttons currently pressed
	*/
	std::vector<int> buttons_pressed;

	/**
	 * @param device_id The device id as provided by the SDL_JOYDEVICEADDED event
	 */
	Joystick(unsigned int device_id);
	~Joystick();

	/**
	 * @brief Sets the state of a button
	 * @param button The button the state of which is to be set
	 * @param state The new state of the button
	 */
	EXPORT void set_button_state(int button, bool state);
	/**
	 * @brief Marks or unmarks a button as a switch.
	 * 	Switches will not be added to the buttons_pressed list.
	 * @param button The button to mark or unmark
	 * @param mark True to mark, false to unmark
	 */
	EXPORT void mark_switch(int button, bool mark = true);
	/**
	 * @brief Sets a dedzone for all axes
	 * @param deadzone The deadzone
	 */
	EXPORT void set_deadzone(unsigned int deadzone);
	/**
	 * @brief Call this function to get the current value of an axis
	 * @param axis The axis to get the value of
	 * @return The current value of the axis
	 */
	EXPORT int get_axis_value(unsigned int axis);
	/**
	 * @brief Call this function to get the current value of a hat
	 * @param hat The hat to get the value of
	 * @return The current value of the hat
	 */
	EXPORT unsigned int get_hat_value(unsigned int hat);
};

}

#endif //__JOYSTICK_H__
