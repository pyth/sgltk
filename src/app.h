#ifndef __CORE_H__
#define __CORE_H__

/**
 * @def check_gl_error(message)
 * @brief Prints out the OpenGL error message, file and line where this macro
 * 	was called as well as the message string passed to it.
 */
#define check_gl_error(message) do{\
	sgltk::App::_check_error(message, __FILE__, __LINE__);\
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
#include <chrono>
#include <thread>
#include <limits>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>
#ifdef HAVE_SDL_TTF_H
	#include <SDL2/SDL_ttf.h>
#endif //HAVE_SDL_TTF_H
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace sgltk {
	/**
	 * @class App
	 * @brief Handles library initialization and provides system information.
	 */
	class App {
		static bool initialized;

		App();
		~App();

		static void _check_error(std::string message, std::string file, unsigned int line);

		public:

		/**
		 * @brief System information
		 */
		static struct SYS_INFO {
			/**
			 * @brief The name of the platform
			 */
			std::string platform_name;
			/**
			 * @brief The number of logical cores
			 */
			int num_logical_cores;
			/**
			 * @brief The amount of ram in MB
			 */
			int system_ram;
			/**
			 * @brief The number of screens
			 */
			int num_screens;
			/**
			 * @brief The major number of the highest OpenGL version
			 * 	that is supported by the system
			 */
			int gl_version_major;
			/**
			 * @brief The minor number of the highest OpenGL version
			 * 	that is supported by the system
			 */
			int gl_version_minor;
		} sys_info;

		/**
		 * @brief A list of all error strings.
		 */
		static std::vector<std::string> error_string;

		/**
		 * @brief Initializes GLEW
		 * @return Returns true on success, flase otherwise
		 */
		static bool init_glew();

		/**
		 * @brief Initializes SDL2_img
		 * @return Returns true on success, flase otherwise
		 */
		static bool init_img();
		static void quit_img();

		/**
		 * @brief Initializes SDL2
		 * @return Returns true on success, flase otherwise
		 */
		static bool init_sdl();
		static void quit_sdl();

#ifdef HAVE_SDL_TTF_H
		/**
		 * @brief Initializes SDL2_ttf
		 * @return Returns true on success, flase otherwise
		 */
		static bool init_ttf();
		static void quit_ttf();
#endif //HAVE_SDL_TTF_H

		/**
		 * @brief Initializes all parts of SDL2 used by SGLTK
		 * @return Returns true on success, flase otherwise
		 */
		static bool init();
		static void quit();

		/**
		 * @brief Gathers system information and populates the sys_info attribute
		 */
		static void get_sys_info();
	};
};

#endif
