#include "core.h"

using namespace std;

bool init_glew() {
	glewExperimental=GL_TRUE;
	if(glewInit()) {
		cerr << "glewInit failed" << endl;
		return false;
	}
	return true;
}

bool init_img() {
	unsigned int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
	if((IMG_Init(flags) & flags) != flags) {
		cerr << "IMG_Init Error: "<< SDL_GetError() <<endl;
		return false;
	}
	return true;
}

void quit_img() {
	IMG_Quit();
}

bool init_sdl() {
	if(SDL_Init(SDL_INIT_EVERYTHING)) {
		cerr << "SDL_Init Error: "<< SDL_GetError() <<endl;
		return false;
	}
	return true;
}

void quit_sdl() {
	SDL_Quit();
}

bool init_ttf() {
	if(TTF_Init()) {
		cerr << "SDL_Init Error: "<< SDL_GetError() <<endl;
		return false;
	}
	return true;
}

void quit_ttf() {
	TTF_Quit();
}


bool init_mixer() {
	unsigned int flags = MIX_INIT_FLAC|MIX_INIT_MP3|MIX_INIT_OGG;
	unsigned int tmp = Mix_Init(flags);
	if((tmp & flags) != flags) {
		cerr << "Mix_Init Error: "<< SDL_GetError() << endl;
		return false;
	}
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096)==-1) {
		cerr << "Mix_OpenAudio Error: "<< SDL_GetError() << endl;
		exit(20);
		return false;
	}
	return true;
}

void quit_mixer() {
	Mix_CloseAudio();
	while(Mix_Init(0)) {
		Mix_Quit();
	}
}

bool init_lib() {
	if(initialized)
		return true;

	if(init_sdl())
		if(init_ttf())
			if(init_img())
				if(init_mixer()) {
					initialized = true;
					return true;
				}

	quit_lib();
	return false;
}

void quit_lib() {
	initialized = false;
	quit_mixer();
	quit_img();
	quit_ttf();
	quit_sdl();
}

bool check_error(const char *file, int line) {
	GLenum err = glGetError();
	if(err != GL_NO_ERROR) {
		cout<<file<<" "<<line<<" "<<gluErrorString(err)<<endl;
	}
}

