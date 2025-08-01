#include <sgltk/joystick.h>

using namespace sgltk;

unsigned int Joystick::id_max;
std::list<unsigned int> Joystick::ids;

Joystick::Joystick(unsigned int device_id) {
	joystick = SDL_JoystickOpen(device_id);
	if(!joystick) {
		std::string error = std::string("Error opening a new joystick: ") +
			SDL_GetError();
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}

	name = std::string(SDL_JoystickName(joystick));

	num_axes = SDL_JoystickNumAxes(joystick);
	num_hats = SDL_JoystickNumHats(joystick);
	num_buttons = SDL_JoystickNumButtons(joystick);
	instance_id = SDL_JoystickInstanceID(joystick);

	deadzone = 0;

	unsigned int i;
	for(i = 0; i <= id_max + 1; i++) {
		if(std::find(std::begin(ids), std::end(ids), i) == std::end(ids)) {
			break;
		}
	}
	id = i;
	ids.push_back(id);
	if(i > id_max)
		id_max = i;
}

Joystick::~Joystick() {
	SDL_JoystickClose(joystick);
	const auto& pos = std::find(std::begin(ids), std::end(ids), id);
	if(pos != std::end(ids)) {
		ids.erase(pos);
	}
}

void Joystick::set_button_state(int button, bool state) {
	std::vector<int>::iterator it;
	if(state) {
		it = std::find(switches.begin(), switches.end(), button);
		if(it == switches.end())
			buttons_pressed.push_back(button);
	}
	else {
		it = std::find(buttons_pressed.begin(),
			buttons_pressed.end(), button);
		if(it != buttons_pressed.end())
			buttons_pressed.erase(it);
	}
}

void Joystick::mark_switch(int button, bool mark) {
	std::vector<int>::iterator it = std::find(switches.begin(),
						  switches.end(),
						  button);

	if(it == switches.end()) {
		if(mark)
			switches.push_back(button);
	} else {
		if(!mark)
			switches.erase(it);
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
