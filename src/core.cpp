#include "core.h"

bool sgltk::initialized = false;

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

#ifdef HAVE_SDL_TTF_H
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
#endif //HAVE_SDL_TTF_H

#ifdef HAVE_SDL_MIXER_H
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
#endif //HAVE_SDL_MIXER_H

bool sgltk::init_lib() {
	if(sgltk::initialized)
		return true;

	if(sgltk::init_sdl())
		if(sgltk::init_img())
#ifdef HAVE_SDL_TTF_H
			if(sgltk::init_ttf())
#endif //HAVE_SDL_TTF_H
#ifdef HAVE_SDL_MIXER_H
				if(sgltk::init_mixer()) {
#endif //HAVE_SDL_MIXER_H
					sgltk::initialized = true;
					return true;
#ifdef HAVE_SDL_MIXER_H
				}
#endif //HAVE_SDL_MIXER_H

	sgltk::quit_lib();
	return false;
}

void sgltk::quit_lib() {
	sgltk::initialized = false;
#ifdef HAVE_SDL_MIXER_H
	quit_mixer();
#endif //HAVE_SDL_MIXER_H
	quit_img();
#ifdef HAVE_SDL_TTF_H
	quit_ttf();
#endif //HAVE_SDL_TTF_H
	quit_sdl();
}

void sgltk::_check_error(std::string message, std::string file, unsigned int line) {
	std::string err_string;
	GLenum err = glGetError();

	switch(err) {
		case GL_INVALID_ENUM:
			err_string = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			err_string = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			err_string = "INVALID_OPERATION";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			err_string = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			err_string = "OUT_OF_MEMORY";
			break;
		case GL_STACK_OVERFLOW:
			err_string = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			err_string = "STACK_UNDERFLOW";
			break;
	}
	while(err != GL_NO_ERROR) {
		std::cout << file << " - " << line << ": " << err_string;
		if(message.length() > 0)
			std::cout << " - " << message;
		std::cout << std::endl;
		err = glGetError();
	}
}

