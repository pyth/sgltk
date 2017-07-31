#include "window.h"

using namespace sgltk;

std::map<unsigned int, Gamepad *> Window::gamepad_instance_id_map;
std::map<unsigned int, Joystick *> Window::joystick_instance_id_map;

Window::Window(const std::string& title, int res_x, int res_y, int offset_x, int offset_y, unsigned int flags) {

	running = true;
	mouse_relative = false;
	keys = SDL_GetKeyboardState(NULL);
	delta_time = 0;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	width = res_x;
	height = res_y;
	window = SDL_CreateWindow(title.c_str(), offset_x, offset_y,
				  res_x, res_y,
				  SDL_WINDOW_OPENGL |
				  SDL_WINDOW_RESIZABLE |
				  flags);
	if(!window) {
		std::string error = std::string("Error opening window: ") + SDL_GetError();
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}
	if(App::gl_version_manual) {
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_maj);
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_min);
		context = SDL_GL_CreateContext(window);
	} else {
		for(gl_maj = 4; gl_maj > 2; gl_maj--) {
			for(gl_min = 5; gl_min >= 0; gl_min--) {
				if(gl_maj == 3 && gl_min > 3) {
					continue;
				}
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_maj);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_min);
				context = SDL_GL_CreateContext(window);
				if(context) {
					goto endloop;
				}
			}
		}
	}
endloop:
	if(!context) {
		SDL_DestroyWindow(window);
		std::string error = std::string("Error creating OpenGL context: ") +
			SDL_GetError();
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}

	glGetIntegerv(GL_MAX_PATCH_VERTICES, &App::sys_info.max_patch_vertices);
	glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &App::sys_info.max_tess_level);

	App::init_glew();
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

Window::~Window() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	keys_pressed.clear();
}

void Window::set_icon(const std::string& filename) {
	Image img(filename.c_str());
	SDL_SetWindowIcon(window, img.image);
}

void Window::set_icon(const Image& icon) {
	SDL_SetWindowIcon(window, icon.image);
}

void Window::set_title(const std::string& title) {
	SDL_SetWindowTitle(window, title.c_str());
}

void Window::set_resizable(bool on) {
	SDL_SetWindowResizable(window, (SDL_bool)on);
}

void Window::take_screenshot(Image& image) {
	char *buf = new char[4 * width * height];
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buf);
	image.load(width, height, 4, buf);
	image.vertical_flip();
	delete buf;
}

void Window::grab_mouse(bool on) {
	SDL_SetWindowGrab(window, (SDL_bool)on);
}

int Window::get_display_index() {
	int ret = SDL_GetWindowDisplayIndex(window);
	if(ret < 0) {
		App::error_string.push_back(std::string("Error on acquiring the "
			"display index: ") + SDL_GetError());
	}
	return ret;
}

bool Window::set_display_mode(const SDL_DisplayMode& mode) {
	if(SDL_SetWindowDisplayMode(window, &mode)) {
		App::error_string.push_back(std::string("Error on changing "
			"window display mode: ") + SDL_GetError());
		return false;
	}
	return true;
}

bool Window::fullscreen_mode(FULLSCREEN_MODE mode) {
	if(SDL_SetWindowFullscreen(window, mode) < 0) {
		App::error_string.push_back(std::string("Error on changing "
		"window fullscreen state: ") + SDL_GetError());
		return false;
	}
	return true;
}

void Window::set_relative_mode(bool on) {
	mouse_relative = on;
	SDL_SetRelativeMouseMode((SDL_bool)on);
}

void Window::set_mouse_position(int x, int y) {
	SDL_WarpMouseInWindow(window, x, y);
}

void Window::set_cursor_visibility(bool show) {
	int toggle = SDL_DISABLE;
	if(show)
		toggle = SDL_ENABLE;

	SDL_ShowCursor(toggle);
}

bool Window::get_cursor_visibility() {
	int ret = SDL_ShowCursor(SDL_QUERY);
	if(ret == SDL_ENABLE)
		return true;

	return false;
}

void Window::poll_events() {
	int value;
	SDL_Event event;
	Gamepad *gamepad;
	Joystick *joystick;
	std::map<unsigned int, Gamepad *>::iterator gamepad_it;
	std::map<unsigned int, Joystick *>::iterator joystick_it;
	std::vector<int>::iterator button;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			handle_exit();
			break;
		case SDL_WINDOWEVENT:
			switch(event.window.event) {
				case SDL_WINDOWEVENT_CLOSE:
					handle_exit();
					break;
				case SDL_WINDOWEVENT_RESIZED:
					width = event.window.data1;
					height = event.window.data2;
					handle_resize();
					break;
			}
			break;
		case SDL_KEYDOWN:
			if(event.key.repeat == 0) {
				keys_pressed.push_back(SDL_GetKeyName(event.key.keysym.sym));
				handle_key_press(SDL_GetKeyName(event.key.keysym.sym), true);
			}
			break;
		case SDL_KEYUP:
			keys_pressed.erase(std::find(keys_pressed.begin(),
				keys_pressed.end(),
				SDL_GetKeyName(event.key.keysym.sym)));
			handle_key_press(SDL_GetKeyName(event.key.keysym.sym), false);
			break;
		case SDL_MOUSEWHEEL:
			handle_mouse_wheel(event.wheel.x, event.wheel.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			if(mouse_relative)
				handle_mouse_button(0, 0, event.button.button,
						    (event.button.state == SDL_PRESSED),
						    event.button.clicks);
			else
				handle_mouse_button(event.button.x, event.button.y,
						    event.button.button,
						    (event.button.state == SDL_PRESSED),
						    event.button.clicks);
			break;
		case SDL_MOUSEMOTION:
			if(mouse_relative)
				handle_mouse_motion(event.motion.xrel,
						    event.motion.yrel);
			else
				handle_mouse_motion(event.motion.x,
						    event.motion.y);
			break;
		case SDL_CONTROLLERDEVICEADDED:
			gamepad = new Gamepad(event.cdevice.which);
			gamepad_instance_id_map[gamepad->instance_id] = gamepad;
			handle_gamepad_added(gamepad->id);
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			gamepad_it = gamepad_instance_id_map.find(event.cdevice.which);
			if(gamepad_it != gamepad_instance_id_map.end()) {
				gamepad = gamepad_it->second;
				gamepad_instance_id_map.erase(gamepad->instance_id);
				handle_gamepad_removed(gamepad->id);
				delete Gamepad::id_map[gamepad->id];
			}
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			gamepad_it = gamepad_instance_id_map.find(event.cdevice.which);
			if(gamepad_it != gamepad_instance_id_map.end()) {
				gamepad = gamepad_it->second;
				gamepad->set_button_state(event.cbutton.button, true);
				handle_gamepad_button_press(gamepad->id,
					event.cbutton.button, true);
			}
			break;
		case SDL_CONTROLLERBUTTONUP:
			gamepad_it = gamepad_instance_id_map.find(event.cdevice.which);
			if(gamepad_it != gamepad_instance_id_map.end()) {
				gamepad = gamepad_it->second;
				gamepad->set_button_state(event.cbutton.button, false);
				handle_gamepad_button_press(gamepad->id,
					event.cbutton.button, false);
			}
			break;
		case SDL_CONTROLLERAXISMOTION:
			gamepad_it = gamepad_instance_id_map.find(event.cdevice.which);
			if(gamepad_it != gamepad_instance_id_map.end()) {
				gamepad = gamepad_it->second;
				handle_gamepad_axis_change(gamepad->id,
					event.caxis.axis,
					gamepad->get_axis_value(event.caxis.axis));
			}
			break;
		case SDL_JOYDEVICEADDED:
			if(!SDL_IsGameController(event.jdevice.which)) {
				joystick = new Joystick(event.jdevice.which);
				joystick_instance_id_map[joystick->instance_id] = joystick;
				handle_joystick_added(joystick->id);
			}
			break;
		case SDL_JOYDEVICEREMOVED:
			joystick_it = joystick_instance_id_map.find(event.jdevice.which);
			if(joystick_it != joystick_instance_id_map.end()) {
				joystick = joystick_it->second;
				joystick_instance_id_map.erase(joystick->instance_id);
				handle_joystick_removed(joystick->id);
				delete Joystick::id_map[joystick->id];
			}
			break;
		case SDL_JOYBUTTONDOWN:
			joystick_it = joystick_instance_id_map.find(event.jdevice.which);
			if(joystick_it != joystick_instance_id_map.end()) {
				joystick = joystick_it->second;
				joystick->set_button_state(event.jbutton.button, true);
				handle_joystick_button_press(joystick->id,
					event.jbutton.button, true);
			}
			break;
		case SDL_JOYBUTTONUP:
			joystick_it = joystick_instance_id_map.find(event.jdevice.which);
			if(joystick_it != joystick_instance_id_map.end()) {
				joystick = joystick_it->second;
				joystick->set_button_state(event.jbutton.button, false);
				handle_joystick_button_press(joystick->id,
					event.jbutton.button, false);
			}
			break;
		case SDL_JOYAXISMOTION:
			joystick_it = joystick_instance_id_map.find(event.jdevice.which);
			if(joystick_it != joystick_instance_id_map.end()) {
				joystick = joystick_it->second;
				handle_joystick_axis_change(joystick->id,
					event.jaxis.axis,
					joystick->get_axis_value(event.jaxis.axis));
			}
			break;
		case SDL_JOYHATMOTION:
			joystick_it = joystick_instance_id_map.find(event.jdevice.which);
			if(joystick_it != joystick_instance_id_map.end()) {
				joystick = joystick_it->second;
				handle_joystick_hat_change(joystick->id,
					event.jhat.hat,
					event.jhat.value);
			}
			break;
		case SDL_JOYBALLMOTION:
			joystick_it = joystick_instance_id_map.find(event.jdevice.which);
			if(joystick_it != joystick_instance_id_map.end()) {
				joystick = joystick_it->second;
				handle_joystick_ball_motion(joystick->id,
					event.jball.ball,
					event.jball.xrel,
					event.jball.yrel);
			}
			break;
		}
	}

	for(std::string key : keys_pressed) {
		handle_keyboard(key);
	}

	for(std::pair<unsigned int, Gamepad *> device : gamepad_instance_id_map) {
		if(!device.second)
			continue;
		for(unsigned int axis = 0; axis < device.second->num_axes; axis++) {
			value = device.second->get_axis_value(axis);
			handle_gamepad_axis(device.second->id, axis, value);
		}
		for(unsigned int button = 0; button < device.second->buttons_pressed.size(); button++) {
			handle_gamepad_button(device.second->id, device.second->buttons_pressed[button]);
		}
	}

	for(std::pair<unsigned int, Joystick *> device : joystick_instance_id_map) {
		if(!device.second)
			continue;
		for(unsigned int axis = 0; axis < device.second->num_axes; axis++) {
			value = device.second->get_axis_value(axis);
			handle_joystick_axis(device.second->id, axis, value);
		}
		for(unsigned int button = 0; button < device.second->buttons_pressed.size(); button++) {
			handle_joystick_button(device.second->id, device.second->buttons_pressed[button]);
		}
		for(unsigned int hat = 0; hat < device.second->num_hats; hat++) {
			handle_joystick_hat(device.second->id, hat, device.second->get_hat_value(hat));
		}
	}
}

void Window::handle_gamepad_added(unsigned int gamepad_id) {
}

void Window::handle_gamepad_removed(unsigned int gamepad_id) {
}

void Window::handle_gamepad_button(unsigned int gamepad_id, int button) {
}

void Window::handle_gamepad_button_press(unsigned int gamepad_id, int button, bool pressed) {
}

void Window::handle_gamepad_axis(unsigned int gamepad_id, unsigned int axis, int value) {
}

void Window::handle_gamepad_axis_change(unsigned int gamepad_id, unsigned int axis, int value) {
}

void Window::handle_joystick_added(unsigned int joystick_id) {
}

void Window::handle_joystick_removed(unsigned int joystick_id) {
}

void Window::handle_joystick_button(unsigned int joystick_id, int button) {
}

void Window::handle_joystick_button_press(unsigned int joystick_id, int button, bool pressed) {
}

void Window::handle_joystick_axis(unsigned int joystick_id, unsigned int axis, int value) {
}

void Window::handle_joystick_axis_change(unsigned int joystick_id, unsigned int axis, int value) {
}

void Window::handle_joystick_hat(unsigned int joystick_id, unsigned int hat, unsigned int value) {
}

void Window::handle_joystick_hat_change(unsigned int joystick_id, unsigned int hat, unsigned int value) {
}

void Window::handle_joystick_ball_motion(unsigned int joystick_id, unsigned int ball, int xrel, int yrel) {
}

void Window::handle_keyboard(const std::string& key) {
}

void Window::handle_key_press(const std::string& key, bool pressed) {
}

void Window::handle_mouse_motion(int x, int y) {
}

void Window::handle_mouse_wheel(int x, int y) {
}

void Window::handle_mouse_button(int x, int y,
			      int button,
			      bool down,
			      int clicks) {
}

void Window::handle_resize() {
}

void Window::handle_exit() {
	stop();
}

void Window::display() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::run(unsigned int fps) {
	double frame_time;
	double time_to_wait;
	Timer frame_timer;
	if(fps < 1)
		frame_time = 0;
	else
		frame_time = 1000.0 / fps;
	running = true;

	display();

	while(running) {
		poll_events();
		if(!window) {
			break;
		}
		frame_timer.start();
		display();
		if(fps > 0) {
			time_to_wait = frame_time - frame_timer.get_time_ms();
			if(delta_time < frame_time) {
				std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(time_to_wait));
			}
		}
		delta_time = frame_timer.get_time_s();
		SDL_GL_SwapWindow(window);
	}
}

void Window::stop() {
	running = false;
}
