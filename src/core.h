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

bool init_glew();

bool init_img();
void quit_img();

bool init_sdl();
void quit_sdl();

bool init_ttf();
void quit_ttf();

bool init_mixer();
void quit_mixer();

bool check_error(const char *file, int line);

#endif
