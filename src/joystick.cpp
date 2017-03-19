#include "joystick.h"

using namespace sgltk;

unsigned int Joystick::id_max;
std::map<unsigned int, Joystick *> Joystick::id_map;

Joystick::Joystick(unsigned int device_id) {
	joystick = SDL_JoystickOpen(device_id);
	if(!joystick) {
		std::string error = std::string("Error opening a new joystick: ") +
			SDL_GetError();
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}

	num_axes = SDL_JoystickNumAxes(joystick);
	num_hats = SDL_JoystickNumHats(joystick);
	num_buttons = SDL_JoystickNumButtons(joystick);
	instance_id = SDL_JoystickInstanceID(joystick);

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

Joystick::~Joystick() {
	SDL_JoystickClose(joystick);
	id_map.erase(id);
}

void Joystick::set_button_state(int button, bool state) {
	if(state) {
		buttons_pressed.push_back(button);
	}
	else {
		std::vector<int>::iterator pressed_button = std::find(buttons_pressed.begin(),
			buttons_pressed.end(), button);
		buttons_pressed.erase(pressed_button);
	}
}

void Joystick::set_deadzone(unsigned int deadzone) {
	this->deadzone = deadzone;
}

int Joystick::get_axis_value(unsigned int axis) {
	int value = SDL_JoystickGetAxis(joystick, (SDL_GameControllerAxis)axis);
	if((unsigned int)std::abs(value) > deadzone)
		return value;
	return 0;
}

unsigned int Joystick::get_hat_value(unsigned int hat) {
	return SDL_JoystickGetHat(joystick, hat);
}
