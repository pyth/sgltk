#ifndef __CORE_H__
#define __CORE_H__

#define check_gl_error(message) do{\
	sgltk::_check_error(message, __FILE__, __LINE__);\
	}while(0)

#ifdef __WIN32__
	#include <windows.h>
#endif

#include "config.h"

#include <GL/glew.h>
#include <GL/glu.h>
/*#ifdef _WIN32
	#include <GL/GL.h>
#else
	#include <GL/gl.h>
#endif*/

#include <string>
#include <limits>
#include <unistd.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#ifdef HAVE_SDL_MIXER_H
	#include <SDL2/SDL_mixer.h>
#endif
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace sgltk {
	static bool initialized;

	/**
	 * @brief The path to the executable
	 */
	static std::string executable_path;

	/**
	 * @brief Initializes GLEW
	 * @return Returns true on success, flase otherwise
	 */
	bool init_glew();

	/**
	 * @brief Initializes SDL2_img
	 * @return Returns true on success, flase otherwise
	 */
	bool init_img();
	void quit_img();

	/**
	 * @brief Initializes SDL2
	 * @return Returns true on success, flase otherwise
	 */
	bool init_sdl();
	void quit_sdl();

	/**
	 * @brief Initializes SDL2_ttf
	 * @return Returns true on success, flase otherwise
	 */
	bool init_ttf();
	void quit_ttf();

#ifdef HAVE_SDL_MIXER_H
	/**
	 * @brief Initializes SDL2_mixer
	 * @return Returns true on success, flase otherwise
	 */
	bool init_mixer();
	void quit_mixer();
#endif

	/**
	 * @brief Initializes all parts of SDL2 used by SGLTK
	 * @return Returns true on success, flase otherwise
	 */
	bool init_lib();
	void quit_lib();

	/**
	 * @brief Returns the path to the executable, that was set by a call to
	 * 	  set_path_to_executable
	 */
	const char *get_path_to_executable(void);
	/**
	 * @brief Extracts the path to the executable from the argument list
	 */
	void set_path_to_executable(char **argv);

	void _check_error(const char *message, char *file, unsigned int line);
};

#endif
