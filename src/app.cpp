#include "app.h"

using namespace std;

App::App(const char* title, int resX, int resY, int offsetX, int offsetY, unsigned int flags) {
	init_sdl();
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	width = resX;
	height = resY;
	window = SDL_CreateWindow(title, offsetX, offsetY, resX, resY, flags);
	if(!window) {
		cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		return;
	}
	//SDL_SetWindowGrab(window, SDL_TRUE);
	//SDL_GL_SetSwapInterval(1);
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

int App::poll_events() {
	SDL_Event event;
	bool keys[SDL_NUM_SCANCODES];

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT:
			return -1;
			break;
		case SDL_KEYDOWN:
			keys[event.key.keysym.scancode] = true;
			break;
		case SDL_KEYUP:
			keys[event.key.keysym.scancode] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			return -1;
			break;
		default:
			return 1;
			break;
		}
	}
	handle_keyboard(keys);
	return 0;
}

void App::handle_keyboard(bool *keys) {
	/*for(int i=0; i<SDL_NUM_SCANCODES; i++) {
		if(keys[i])
			cout<<SDL_GetScancodeName((SDL_Scancode)i)<<" ";
	}
	cout<<endl;*/
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

	while(running) {
		frame_timer.start();
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
				SDL_Delay(1000 / fps - frame_time);
			}
		}
		SDL_GL_SwapWindow(window);
	}
}
