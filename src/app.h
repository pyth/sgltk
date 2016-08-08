#ifndef __APP_H__
#define __APP_H__

/**
 * @def check_gl_error(message)
 * @brief Prints out the OpenGL error message, file and line where this macro
 * 	was called as well as the message string passed to it.
 */
#define check_gl_error(message) do{\
	sgltk::App::_check_error(message, __FILE__, __LINE__);\
}while(0)

#ifdef _WIN32
	#ifdef MAKE_LIB
		#ifdef MAKE_DLL
			#define EXPORT __declspec(dllexport)
		#elif MAKE_STATIC
			#define EXPORT
		#endif
	#else
		#ifdef DYNAMIC
			#define EXPORT __declspec(dllimport)
		#elif STATIC
			#define EXPORT
		#endif
	#endif
#else
	#define EXPORT
#endif

#include "config.h"

#include <GL/glew.h>
#include <GL/glu.h>

#include <string>
#include <chrono>
#include <thread>
#include <limits>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <exception>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SDL_MAIN_HANDLED
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
		EXPORT static struct SYS_INFO {
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
			 * @brief The number of displays
			 */
			int num_displays;
			/**
			 * @brief The display modes of each display
			 */
			std::vector<SDL_DisplayMode> desktop_display_modes;
			/**
			 * @brief A list of other supported display modes for every
			 * 	detected display
			 */
			std::vector<std::vector<SDL_DisplayMode> > supported_display_modes;
			/**
			 * @brief Display bounds
			 */
			std::vector<SDL_Rect> display_bounds;
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
		EXPORT static std::vector<std::string> error_string;

		/**
		 * @brief Initializes GLEW
		 * @return Returns true on success, flase otherwise
		 */
		EXPORT static bool init_glew();

		/**
		 * @brief Initializes SDL2_img
		 * @return Returns true on success, flase otherwise
		 */
		EXPORT static bool init_img();
		EXPORT static void quit_img();

		/**
		 * @brief Initializes SDL2
		 * @return Returns true on success, flase otherwise
		 */
		EXPORT static bool init_sdl();
		EXPORT static void quit_sdl();

#ifdef HAVE_SDL_TTF_H
		/**
		 * @brief Initializes SDL2_ttf
		 * @return Returns true on success, flase otherwise
		 */
		EXPORT static bool init_ttf();
		EXPORT static void quit_ttf();
#endif //HAVE_SDL_TTF_H

		/**
		 * @brief Initializes all parts of SDL2 used by SGLTK
		 * @return Returns true on success, flase otherwise
		 */
		EXPORT static bool init();
		EXPORT static void quit();

		/**
		* @brief Enables the screensaver
		*/
		EXPORT void enable_screensaver();

		/**
		* @brief Disables the screensaver
		*/
		EXPORT void disable_screensaver();

		/**
		* @brief Turns VSync on or off
		* @param on	True turns VSync on, false turns it off
		* @return	Returns false if VSync is not supported, true otherwise
		*/
		EXPORT bool enable_vsync(bool on);

		/**
		 * @brief Gathers system information and populates the sys_info attribute
		 */
		EXPORT static void get_sys_info();
	};
};

#endif
