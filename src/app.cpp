#include "app.h"

using namespace std;

App::App(const char* title, int res_x, int res_y, int offset_x, int offset_y,
	 int gl_maj, int gl_min, unsigned int flags) {
	init_sdl();

	SDL_DisableScreenSaver();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_maj);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_min);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	width = res_x;
	height = res_y;
	window = SDL_CreateWindow(title, offset_x, offset_y,
				  res_x, res_y, flags);
	if(!window) {
		cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		return;
	}
	context = SDL_GL_CreateContext(window);
	init_glew();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

App::~App() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	quit_ttf();
	quit_sdl();
}

void App::grab_mouse(SDL_bool on) {
	SDL_SetWindowGrab(window, on);
}

bool App::enable_vsync(bool on) {
	if(on) {
		return SDL_GL_SetSwapInterval(1);
	} else {
		return SDL_GL_SetSwapInterval(0);
	}
}

int App::poll_events() {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			return -1;
			break;
		case SDL_KEYDOWN:
			handle_keyboard(SDL_GetKeyName(event.key.keysym.sym),
					get_modifier(event.key.keysym.mod),
					true);
			break;
		case SDL_KEYUP:
			handle_keyboard(SDL_GetKeyName(event.key.keysym.sym),
					get_modifier(event.key.keysym.mod),
					false);
			break;
		case SDL_MOUSEBUTTONDOWN:
			return -1;
			break;
		default:
			return 1;
			break;
		}
	}
	return 0;
}

void App::handle_keyboard(const char *key, const char *mod, bool down) {
}

void App::display() {
	glClearColor(1.0, 0.0, 0.0, 1.0);
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
		if(poll_events() < 0) {
			running = false;
		}
		display();
		if(!window) {
			break;
		}
		if(fps > 0) {
			frame_time = frame_timer.get_time();
			if(frame_time < 1000 / fps) {
				continue;
			}
		}
		SDL_GL_SwapWindow(window);
		frame_timer.start();
	}
}
