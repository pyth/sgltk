#include "app.h"

using namespace sgltk;

bool App::initialized = false;
bool App::gl_version_manual = false;
struct SYS_INFO App::sys_info;
std::vector<std::string> App::error_string = {};

bool App::init_glew() {
	glewExperimental=GL_TRUE;
	if(glewInit()) {
		App::error_string.push_back("glewInit failed");
		return false;
	}
	return true;
}

bool App::init_img() {
	unsigned int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
	if((IMG_Init(flags) & flags) != flags) {
		App::error_string.push_back(std::string("IMG_Init Error: ") +
							SDL_GetError());
		return false;
	}
	return true;
}

void App::quit_img() {
	IMG_Quit();
}

bool App::init_sdl() {
	if(SDL_Init(SDL_INIT_VIDEO |
			SDL_INIT_JOYSTICK |
			SDL_INIT_HAPTIC |
			SDL_INIT_GAMECONTROLLER)) {
		App::error_string.push_back(std::string("SDL_Init Error: ") +
							SDL_GetError());
		return false;
	}
	return true;
}

void App::quit_sdl() {
	SDL_Quit();
}

#ifdef HAVE_SDL_TTF_H
bool App::init_ttf() {
	if(TTF_Init()) {
		App::error_string.push_back(std::string("SDL_Init Error: ") +
							SDL_GetError());
		return false;
	}
	return true;
}

void App::quit_ttf() {
	TTF_Quit();
}
#endif //HAVE_SDL_TTF_H

bool App::init() {
	if(App::initialized)
		return true;

	if(App::init_sdl())
		if(App::init_img()) {
#ifdef HAVE_SDL_TTF_H
			if(App::init_ttf()) {
#endif //HAVE_SDL_TTF_H
				App::initialized = true;
				App::get_sys_info();
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
				SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
				return true;
#ifdef HAVE_SDL_TTF_H
			}
#endif //HAVE_SDL_TTF_H
		}

	App::quit();
	return false;
}

void App::set_gl_version(int major, int minor) {
	int maj;
	int min;
	if(major < 3) {
		maj = 3;
		min = 0;
		App::error_string.push_back("Unsupported version number. Defaulting to version 3.0");
	} else if(major > 4) {
		maj = 3;
		min = 0;
		App::error_string.push_back("Unsupported version number. Defaulting to version 3.0");
	} else if(major == 3) {
		if(minor < 0) {
			maj = 3;
			min = 0;
			App::error_string.push_back("Unsupported version number. Defaulting to version 3.0");
		} else if(minor > 3) {
			maj = 3;
			min = 0;
			App::error_string.push_back("Unsupported version number. Defaulting to version 3.0");
		} else {
			maj = major;
			min = minor;
		}
	} else if(major == 4) {
		if(minor < 0) {
			maj = 3;
			min = 0;
			App::error_string.push_back("Unsupported version number. Defaulting to version 3.0");
		} else if(minor > 5) {
			maj = 3;
			min = 0;
			App::error_string.push_back("Unsupported version number. Defaulting to version 3.0");
		} else {
			maj = major;
			min = minor;
		}
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, maj);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, min);

	gl_version_manual = true;
}

void App::set_depth_stencil_size(int depth_size, int stencil_size) {
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth_size);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencil_size);
}

void App::set_msaa_sample_number(int number_samples) {
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, number_samples);
}

void App::quit() {
	App::initialized = false;
	gl_version_manual = false;
	App::quit_img();
#ifdef HAVE_SDL_TTF_H
	App::quit_ttf();
#endif //HAVE_SDL_TTF_H
	App::quit_sdl();
}

sgltk::App::~App() {
	error_string.clear();
}

void App::enable_screensaver() {
	SDL_EnableScreenSaver();
}

void App::disable_screensaver() {
	SDL_DisableScreenSaver();
}

bool App::enable_vsync(bool on) {
	int ret;
	if (on) {
		ret = SDL_GL_SetSwapInterval(-1);
		if (!ret) {
			ret = SDL_GL_SetSwapInterval(1);
		}
	}
	else {
		ret = SDL_GL_SetSwapInterval(0);
	}
	return (ret == 1);
}

void App::_check_error(std::string message, std::string file, unsigned int line) {
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

void App::get_sys_info() {
	sys_info.platform_name = std::string(SDL_GetPlatform());

	sys_info.num_logical_cores = SDL_GetCPUCount();
	sys_info.system_ram = SDL_GetSystemRAM();

	sys_info.num_displays = SDL_GetNumVideoDisplays();
	sys_info.desktop_display_modes.resize(sys_info.num_displays);
	sys_info.supported_display_modes.resize(sys_info.num_displays);
	for(int i = 0; i < sys_info.num_displays; i++) {
		int num_modes = SDL_GetNumDisplayModes(i);
		sys_info.supported_display_modes[i].resize(num_modes);
		SDL_GetDesktopDisplayMode(i, &sys_info.desktop_display_modes[i]);
		for(int j = 0; j < num_modes; j++)
			SDL_GetDisplayMode(i, j, &sys_info.supported_display_modes[i][j]);
	}
	sys_info.display_bounds.resize(sys_info.num_displays);
	for(int i = 0; i < sys_info.num_displays; i++)
		SDL_GetDisplayBounds(i, &sys_info.display_bounds[i]);
}
