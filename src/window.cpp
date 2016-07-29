#include "window.h"

using namespace sgltk;

Window::Window(const char* title, int res_x, int res_y,
		int offset_x, int offset_y,
		int gl_maj, int gl_min,
		unsigned int flags) {

	running = true;
	mouse_relative = false;
	keys = SDL_GetKeyboardState(NULL);
	delta_time = 0;

	int glmaj = gl_maj;
	int glmin = gl_min;
	if(gl_min < 0) {
		App::error_string.push_back("Minor version number cannot be"
			" negative. Defaulting to version x.0");
		glmin = 0;
	}
	if(gl_maj < 3) {
		App::error_string.push_back("sgltk requires opengl version 3.0"
			" or newer. Defaulting to version 3.0");
		glmaj = 3;
		glmin = 0;
	} else if(gl_maj == 3 && gl_min > 3) {
		App::error_string.push_back("Did you mean 3.3?"
			" Defaulting to version 3.3");
		glmin = 3;
	} else if(gl_maj == 4 && gl_min > 5) {
		App::error_string.push_back("Did you mean 4.5?"
			" Defaulting to version 4.5");
		glmin = 5;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glmaj);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glmin);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	width = res_x;
	height = res_y;
	window = SDL_CreateWindow(title, offset_x, offset_y,
				  res_x, res_y,
				  SDL_WINDOW_OPENGL |
				  SDL_WINDOW_RESIZABLE |
				  flags);
	if(!window) {
		std::string error = std::string("Error opening window: ") + SDL_GetError();
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}
	context = SDL_GL_CreateContext(window);
	if(!context) {
		SDL_DestroyWindow(window);
		std::string error = std::string("Error creating OpenGL context: ") +
			SDL_GetError();
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}
	App::init_glew();
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

Window::~Window() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

void Window::set_icon(Image *icon) {
	SDL_SetWindowIcon(this->window, icon->image);
}

void Window::enable_screensaver() {
	SDL_EnableScreenSaver();
}

void Window::disable_screensaver() {
	SDL_DisableScreenSaver();
}

void Window::grab_mouse(bool on) {
	SDL_SetWindowGrab(window, (SDL_bool)on);
}

void Window::set_relative_mode(bool on) {
	mouse_relative = on;
	SDL_SetRelativeMouseMode((SDL_bool)on);
}

bool Window::enable_vsync(bool on) {
	int ret;
	if(on) {
		ret = SDL_GL_SetSwapInterval(-1);
		if(!ret) {
			ret = SDL_GL_SetSwapInterval(1);
		}
	} else {
		ret = SDL_GL_SetSwapInterval(0);
	}
	return (ret == 1);
}

void Window::poll_events() {
	SDL_Event event;
	Gamepad *gamepad;

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
		case SDL_KEYUP:
			break;
		case SDL_MOUSEWHEEL:
			handle_mouse_wheel(event.wheel.x, event.wheel.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			if(mouse_relative)
				handle_mouse_button(0, 0, (MOUSE_BUTTON)event.button.button,
						    (event.button.state == SDL_PRESSED),
						    event.button.clicks);
			else
				handle_mouse_button(event.button.x, event.button.y,
						    (MOUSE_BUTTON)event.button.button,
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
			gamepad = new sgltk::Gamepad(event.cdevice.which);
			handle_gamepad_added(gamepad->id);
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			gamepad = sgltk::Gamepad::instance_id_map[event.cdevice.which];
			handle_gamepad_removed(gamepad->id);
			delete sgltk::Gamepad::id_map[gamepad->id];
			break;
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			gamepad = sgltk::Gamepad::instance_id_map[event.cdevice.which];
			handle_gamepad_button(gamepad->id,
				event.cbutton.button,
				(event.cbutton.state == SDL_PRESSED));
			break;
		case SDL_CONTROLLERAXISMOTION:
			gamepad = sgltk::Gamepad::instance_id_map[event.cdevice.which];
			handle_gamepad_axis(gamepad->id,
				event.caxis.axis,
				event.caxis.value);
		}
	}
	handle_keyboard();
}

void Window::handle_gamepad_added(unsigned int gamepad_id) {
}

void Window::handle_gamepad_removed(unsigned int gamepad_id) {
}

void Window::handle_gamepad_button(unsigned int gamepad_id, int button, bool pressed) {
}

void Window::handle_gamepad_axis(unsigned int gamepad_id, unsigned int axis, int value) {
}

void Window::handle_keyboard() {
}

bool Window::key_pressed(const char *key) {
	if(keys[SDL_GetScancodeFromName(key)]) {
		return true;
	}
	return false;
}

void Window::handle_mouse_motion(int x, int y) {
}

void Window::handle_mouse_wheel(int x, int y) {
}

void Window::handle_mouse_button(int x, int y,
			      MOUSE_BUTTON button,
			      bool down,
			      int clicks) {
}

void Window::handle_resize() {
}

void Window::handle_exit() {
	running = false;
}

void Window::display() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::run() {
	run(0);
}

void Window::run(int fps) {
	double frame_time;
	Timer frame_timer;
	if(fps < 1)
		frame_time = 1e-30;
	else
		frame_time = 1000.0 / fps;
	bool running = true;

	while(running) {
		frame_timer.start();
		poll_events();
		if(!window) {
			break;
		}
		display();
		delta_time = frame_timer.get_time();
		if(fps > 0) {
			if(delta_time < frame_time) {
				//SDL_Delay(frame_time - delta_time);
				std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(frame_time - delta_time));
			}
		}
		delta_time = frame_timer.get_time();
		SDL_GL_SwapWindow(window);
	}
}
