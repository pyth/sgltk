#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include "app.h"

namespace sgltk {

/**
 * @class Manages and handles gamepads
 */
class Gamepad {
	SDL_GameController *gamepad;
	SDL_Haptic *haptic;
	unsigned int instance_id;
	static unsigned int id_max;

	public:
	/**
	 * @brief The gamepad id
	 */
	int id;
	/**
	 * @brief Maps a gamepad id to a gamepad object
	 */
	EXPORT static std::map<unsigned int, Gamepad *> id_map;
	/**
	 * @brief Maps the instance ids as provided by SDL2 events to a gamepad objects
	 */
	EXPORT static std::map<unsigned int, Gamepad *> instance_id_map;

	/**
	 * @param device_id The device id as provided by the SDL_CONTROLLERDEVICEADDED event
	 */
	EXPORT Gamepad(unsigned int device_id);
	EXPORT ~Gamepad();

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

#endif
