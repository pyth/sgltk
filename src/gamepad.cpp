#include "gamepad.h"

using namespace sgltk;

unsigned int Gamepad::id_max;
std::map<unsigned int, Gamepad *> Gamepad::id_map;

Gamepad::Gamepad(unsigned int device_id) {
	gamepad = SDL_GameControllerOpen(device_id);
	if(!gamepad) {
		std::string error = std::string("Error opening a new gamepad: ") +
			SDL_GetError();
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}

	joystick = SDL_GameControllerGetJoystick(gamepad);
	instance_id = SDL_JoystickInstanceID(joystick);

	num_axes = SDL_JoystickNumAxes(joystick);
	num_buttons = SDL_JoystickNumButtons(joystick);

	if(SDL_JoystickIsHaptic(joystick)) {
		haptic = SDL_HapticOpenFromJoystick(joystick);
		if(!haptic) {
			std::string error = std::string("Error opening a new haptic device: ") +
				SDL_GetError();
			App::error_string.push_back(error);
			throw std::runtime_error(error);
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
	SDL_HapticClose(haptic);
	SDL_GameControllerClose(gamepad);
	id_map.erase(id);
}

void Gamepad::set_button_state(int button, bool state) {
	if(state) {
		buttons_pressed.push_back(button);
	} else {
		std::vector<int>::iterator pressed_button = std::find(buttons_pressed.begin(),
			buttons_pressed.end(), button);
		buttons_pressed.erase(pressed_button);
	}
}

void Gamepad::set_deadzone(unsigned int deadzone) {
	this->deadzone = deadzone;
}

int Gamepad::get_axis_value(unsigned int axis) {
	int value = SDL_GameControllerGetAxis(gamepad, (SDL_GameControllerAxis)axis);
	if((unsigned int)std::abs(value) > deadzone)
		return value;
	return 0;
}

void Gamepad::play_rumble(float magnitude, unsigned int duration) {
	SDL_HapticRumblePlay(haptic, magnitude, duration);
}

void Gamepad::stop_rumble() {
	SDL_HapticRumbleStop(haptic);
}
