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

	EXPORT Renderbuffer();
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
	/**
	 * @brief Sets the format of the renderbuffer
	 * @param format The internal format of the buffer
	 */
	EXPORT void set_format(GLenum format);
	/**
	 * @brief Sets the width and height of the renderbuffer
	 * @param height The width of the buffer
	 * @param width The height of the buffer
	 */
	EXPORT void set_size(unsigned int width, unsigned int height);
};

}

#endif
