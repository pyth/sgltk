#include "gamepad.h"

using namespace sgltk;

unsigned int Gamepad::id_max;
std::map<unsigned int, Gamepad *> Gamepad::id_map;
std::map<unsigned int, Gamepad *> Gamepad::instance_id_map;

Gamepad::Gamepad(unsigned int device_id) {
	gamepad = SDL_GameControllerOpen(device_id);
	if(!gamepad) {
		App::error_string.push_back(std::string("Error opening a new "
			"game controller: ") + SDL_GetError());
		//subject to change
		throw -1;
	}

	SDL_Joystick *joystick = SDL_GameControllerGetJoystick(gamepad);
	instance_id = SDL_JoystickInstanceID(joystick);
	instance_id_map[instance_id] = this;

	if(SDL_JoystickIsHaptic(joystick)) {
		haptic = SDL_HapticOpenFromJoystick(joystick);
		if(!haptic) {
			App::error_string.push_back(std::string("Error opening"
				" a new haptic device: ") + SDL_GetError());
			throw -2;
		}
		if(SDL_HapticRumbleSupported(haptic) == SDL_TRUE)
			SDL_HapticRumbleInit(haptic);
	} else {
		haptic = NULL;
	}

	unsigned int i;
	for(i = 0; i < id_max + 2; i++) {
		if(id_map.find(i) == id_map.end()) {
			break;
		}
	}
	id = i;
	id_map[i] = this;
	if(i > id_max)
		id_max = i;
}

Gamepad::~Gamepad() {
	id_map.erase(id);
	instance_id_map.erase(instance_id);
	SDL_HapticClose(haptic);
	SDL_GameControllerClose(gamepad);
}

void Gamepad::play_rumble(float magnitude, unsigned int duration) {
	SDL_HapticRumblePlay(haptic, magnitude, duration);
}

void Gamepad::stop_rumble() {
	SDL_HapticRumbleStop(haptic);
}
