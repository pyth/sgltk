#include "app.h"

using namespace std;

App::App(const char* title, int res_x, int res_y, int offset_x, int offset_y,
	 int gl_maj, int gl_min, unsigned int flags) {
	init_lib();

	running = true;
	mouse_relative = false;
	keys = SDL_GetKeyboardState(NULL);

	SDL_DisableScreenSaver();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_maj);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_min);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	width = res_x;
	height = res_y;
	window = SDL_CreateWindow(title, offset_x, offset_y,
				  res_x, res_y, SDL_WINDOW_OPENGL | flags);
	if(!window) {
		cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		return;
	}
	context = SDL_GL_CreateContext(window);
	init_glew();
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

App::~App() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	quit_lib();
}

void App::grab_mouse(bool on) {
	SDL_SetWindowGrab(window, (SDL_bool)on);
}

void App::set_relative_mode(bool on) {
	mouse_relative = on;
	SDL_SetRelativeMouseMode((SDL_bool)on);
}

bool App::enable_vsync(bool on) {
	bool ret;
	if(on) {
		ret = SDL_GL_SetSwapInterval(-1);
		if(!ret) {
			ret = SDL_GL_SetSwapInterval(1);
		}
	} else {
		ret = SDL_GL_SetSwapInterval(0);
	}
	return ret;
}

void App::poll_events() {
	int x, y;
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_WINDOWEVENT:
			if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
				width = event.window.data1;
				height = event.window.data2;
				handle_resize();
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
		}
	}
	handle_keyboard();
}

void App::handle_keyboard() {
}

bool App::key_pressed(const char *key) {
	if(keys[SDL_GetScancodeFromName(key)]) {
		return true;
	}
	return false;
}

void App::handle_mouse_motion(int x, int y) {
}

void App::handle_mouse_wheel(int x, int y) {
}

void App::handle_mouse_button(int x, int y, MOUSE_BUTTON button, bool down,
			      int clicks) {
}

void App::handle_resize() {
}

void App::display() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void App::run() {
	run(0);
}

void App::run(int fps) {
	Timer frame_timer;
	unsigned int frame_time;
	bool running = true;

	frame_timer.start();
	while(running) {
		if(!window) {
			break;
		}
		if(fps > 0) {
			frame_time = frame_timer.get_time();
			if(frame_time < 1000 / fps) {
				continue;
			}
		}
		poll_events();
		display();
		SDL_GL_SwapWindow(window);
		frame_timer.start();
	}
}
