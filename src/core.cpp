#include "core.h"

bool sgltk::init_glew() {
	glewExperimental=GL_TRUE;
	if(glewInit()) {
		std::cerr << "glewInit failed" << std::endl;
		return false;
	}
	return true;
}

bool sgltk::init_img() {
	unsigned int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
	if((IMG_Init(flags) & flags) != flags) {
		std::cerr << "IMG_Init Error: "<< SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

void sgltk::quit_img() {
	IMG_Quit();
}

bool sgltk::init_sdl() {
	if(SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << "SDL_Init Error: "<< SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

void sgltk::quit_sdl() {
	SDL_Quit();
}

bool sgltk::init_ttf() {
	if(TTF_Init()) {
		std::cerr << "SDL_Init Error: "<< SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

void sgltk::quit_ttf() {
	TTF_Quit();
}


bool sgltk::init_mixer() {
	unsigned int flags = MIX_INIT_FLAC|MIX_INIT_MP3|MIX_INIT_OGG;
	unsigned int tmp = Mix_Init(flags);
	if((tmp & flags) != flags) {
		std::cerr << "Mix_Init Error: "<< SDL_GetError() << std::endl;
		return false;
	}
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096)==-1) {
		std::cerr << "Mix_OpenAudio Error: "<< SDL_GetError() << std::endl;
		exit(20);
		return false;
	}
	return true;
}

void sgltk::quit_mixer() {
	Mix_CloseAudio();
	while(Mix_Init(0)) {
		Mix_Quit();
	}
}

bool sgltk::init_lib() {
	if(sgltk::initialized)
		return true;

	if(sgltk::init_sdl())
		if(sgltk::init_img())
			if(sgltk::init_ttf())
				if(sgltk::init_mixer()) {
					sgltk::initialized = true;
					return true;
				}

	sgltk::quit_lib();
	return false;
}

void sgltk::quit_lib() {
	sgltk::initialized = false;
	quit_mixer();
	quit_img();
	quit_ttf();
	quit_sdl();
}

const char *sgltk::get_path_to_executable(void) {
	return sgltk::executable_path.c_str();
}

void sgltk::set_path_to_executable(char **argv) {
	char buf[PATH_MAX];

	getcwd(buf, PATH_MAX);
	sgltk::executable_path = buf;
	sgltk::executable_path += argv[0];

	sgltk::executable_path = executable_path.substr(0, executable_path.find_last_of("/\\"));
}

void sgltk::_check_error(const char *message, char *file, unsigned int line) {
	GLenum err = glGetError();
	while(err != GL_NO_ERROR) {
		std::cout << file << " - " << line << ": " << gluErrorString(err);
		if(strlen(message) > 1)
			std::cout << " - " << message;
		std::cout << std::endl;
		err = glGetError();
	}
}

