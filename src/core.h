#ifndef __CORE_H__
#define __CORE_H__

#define check_gl_error(message) do{\
	_check_error(message, __FILE__, __LINE__);\
	}while(0)

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

#include <string>
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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

bool _check_error(const char *message, const char *file, int line);

#endif
