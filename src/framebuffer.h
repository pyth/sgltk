#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "app.h"
#include "texture.h"
#include "renderbuffer.h"

namespace sgltk {

class Framebuffer {
	GLuint buffer;
	GLint target;
	unsigned int width;
	unsigned int height;
	bool color_attachment;
	bool depth_attachment;
	bool stencil_attachment;
public:
	EXPORT Framebuffer(GLuint target = GL_FRAMEBUFFER);
	EXPORT ~Framebuffer();
	/**
	 * @brief Checks the completeness status of the framebuffer
	 * @return Returns true if the framebuffer is complete, false otherwise
	 */
	EXPORT bool check_buffer_status();
	/**
	 * @brief Sets the framebuffer as render target
	 */
	EXPORT void bind();
	/**
	 * @brief Restores the original framebuffer as render target
	 */
	EXPORT void unbind();
	/**
	 * @brief Attaches a texture to the framebuffer
	 * @param attachment The attachment point to which the texture is attached
	 * @param texture The texture to attach
	 */
	EXPORT void attach_texture(GLenum attachment,
				   Texture& texture);
	/**
	 * @brief Attaches a renderbuffer to the framebuffer
	 * @param attachment The attachment point to which
	 * 	  the renderbuffer is attached
	 * @param buffer The renderbuffer to attach
	 */
	EXPORT void attach_renderbuffer(GLenum attachment,
					Renderbuffer& buffer);
};

}

#endif
