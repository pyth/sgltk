#ifndef __RENDERBUFFER_H__
#define __RENDERBUFFER_H__

#include "app.h"

namespace sgltk {

/**
 * @class Renderbuffer
 * @brief Manages renderbuffers
 */
class Renderbuffer {
	GLenum format;
	public:
	/**
	 * @brief The width of the buffer
	 */
	unsigned int width;
	/**
	 * @brief The height of the buffer
	 */
	unsigned int height;
	/**
	 * @brief The renderbuffer name
	 */
	GLuint buffer;
	/**
	 * @param height The width of the buffer
	 * @param width The height of the buffer
	 * @param format The internal format of the buffer
	 */
	EXPORT Renderbuffer(unsigned int height,
			    unsigned int width,
			    GLenum format);
	EXPORT ~Renderbuffer();
	/**
	 * @brief Binds the renderbuffer
	 */
	EXPORT void bind();
	/**
	 * @brief Unbinds the renderbuffer
	 */
	EXPORT void unbind();
};

}

#endif
