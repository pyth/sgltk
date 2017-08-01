#ifndef __APP_H__
#define __APP_H__
/** @file */

/**
 * Prints out the OpenGL error message, file and line where this macro
 * 	was called as well as the message string passed to it.
 */
#define check_gl_error(message) do{\
	sgltk::App::_check_error(message, __FILE__, __LINE__);\
} while(0)

#ifdef _WIN32
	#ifdef MAKE_LIB //building sgltk
		#ifdef MAKE_DLL //dynamic
			#define EXPORT __declspec(dllexport)
		#elif MAKE_STATIC //static
			#define EXPORT
		#endif //MAKE_DLL
	#else //linking with sgltk
		#ifndef SGLTK_STATIC //static
			#define EXPORT __declspec(dllimport)
		#else //dynamic
			#define EXPORT
		#endif
	#endif //MAKE_LIB
#else //linux
	#define EXPORT
#endif //_WIN32

#include "config.h"

#include <GL/glew.h>

#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <exception>

#ifdef _WIN32 //windows
	#include <direct.h>
#else //linux
	#include <unistd.h>
#endif //_WIN32

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

#ifdef HAVE_ASSIMP_H
	#include <assimp/Importer.hpp>
	#include <assimp/scene.h>
	#include <assimp/postprocess.h>
#endif //HAVE_ASSIMP_H

namespace sgltk {

/**
 * @brief System information
 */
struct SYS_INFO {
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
	 * @brief The maximum number of vertices in a patch
	 */
	int max_patch_vertices;
	/**
	 * @brief The maximum supported tessellation level
	 */
	int max_tess_level;
};


/**
 * @class App
 * @brief Handles library initialization and provides system information.
 */
class App {
	static bool initialized;

	EXPORT App();
	EXPORT ~App();

public:
	/**
	 * @brief True if set_gl_version was called, false otherwise
	 */
	EXPORT static bool gl_version_manual;
	/**
	 * @brief System information
	 */
	EXPORT static struct SYS_INFO sys_info;
	/**
	 * @brief A list of all error strings.
	 */
	EXPORT static std::vector<std::string> error_string;
	/**
	 * @brief Initializes SGLTK
	 * @return Returns true on success, false otherwise
	 */
	EXPORT static bool init();
	/**
	 * @brief Deinitializes SGLTK
	 */
	EXPORT static void quit();
	/**
	 * @brief Initializes GLEW
	 * @return Returns true on success, false otherwise
	 */
	EXPORT static bool init_glew();
	/**
	 * @brief Initializes SDL2_img
	 * @return Returns true on success, false otherwise
	 */
	EXPORT static bool init_img();
	/**
	 * @brief Deinitializes SDL2_img
	 */
	EXPORT static void quit_img();
	/**
	 * @brief Initializes SDL2
	 * @return Returns true on success, false otherwise
	 */
	EXPORT static bool init_sdl();
	/**
	 * @brief Deinitializes SDL2
	 */
	EXPORT static void quit_sdl();

#ifdef HAVE_SDL_TTF_H
	/**
	 * @brief Initializes SDL2_ttf
	 * @return Returns true on success, false otherwise
	 */
	EXPORT static bool init_ttf();
	/**
	 * @brief Deinitializes SDL2_ttf
	 */
	EXPORT static void quit_ttf();
#endif //HAVE_SDL_TTF_H
	/**
	 * @brief Sets the OpenGL version
	 * @param major The major version number
	 * @param minor The minor version number
	 * @note This function needs to be called before a window is created.
		If the parameter combination is not valid, version defaults to 3.0
	 */
	EXPORT static void set_gl_version(int major, int minor);
	/**
	 * @brief Sets the size of the depth and stencil buffers
	 * @param depth_size The minimum size of the depth buffer in bits
	 * @param stencil_size The minimum size of the stencil buffer in bits
	 * @note This function needs to be called before a window is created.
	 */
	EXPORT static void set_depth_stencil_size(int depth_size, int stencil_size);
	/**
	 * @brief Sets the number of samples used for multisample anti-aliasing
	 * @param number_samples The number of samples to be used
	 */
	EXPORT static void set_msaa_sample_number(int number_samples);
	/**
	* @brief Enables the screensaver
	* @param enable True to enable or false to disable the screensaver
	*/
	EXPORT static void enable_screensaver(bool enable);
	/**
	* @brief Turns VSync on or off
	* @param on	True turns VSync on, false turns it off
	* @return	Returns false if VSync is not supported, true otherwise
	*/
	EXPORT static bool enable_vsync(bool on);
	/**
	 * @brief Changes the current working directory to the directory
	 * 	containing the executable
	 * @param argv The array of command line arguments passed to the main
	 * 	function
	 * @return Returns true on success, false otherwise
	 */
	EXPORT static bool chdir_to_bin(char **argv);
	/**
	 * @brief Gathers system information and populates the sys_info attribute
	 */
	EXPORT static void get_sys_info();
	/**
	 * @brief Outputs OpenGL error messages
	 * @note Dont't call this function directly,
	 * 	use check_gl_error(error_message) instead
	 * @see check_gl_error
	 */
	EXPORT static void _check_error(std::string message, std::string file, unsigned int line);

};

};

#endif
