#ifndef __INIT_H__
#define __INIT_H__

#define check_gl_error() check_error(__FILE__,__LINE__)

#ifdef __WIN32__
	#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/glu.h>
/*#ifdef _WIN32
	#include <GL/GL.h>
#else
	#include <GL/gl.h>
#endif*/

#include <iostream>
#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

using namespace std;

static bool initialized;

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

/**
 * @brief Initializes SDL2_mixer
 * @return Returns true on success, flase otherwise
 */
bool init_mixer();
void quit_mixer();

/**
 * @brief Initializes all parts of SDL2 used by SGLTK
 * @return Returns true on success, flase otherwise
 */
bool init_lib();
void quit_lib();

bool check_error(const char *file, int line);

const char *get_modifier(Uint16 mod);

#endif
