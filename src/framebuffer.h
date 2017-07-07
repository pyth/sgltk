#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "app.h"
#include "texture.h"
#include "renderbuffer.h"

namespace sgltk {

/**
 * @class Framebuffer
 * @brief Manages framebuffers
 */
class Framebuffer {
	GLuint buffer;
	GLenum target;
	unsigned int width;
	unsigned int height;
	std::vector<GLenum> draw_buffers;

	static int max_color_attachments;
public:
	/**
	 * @param target The target of the future binding operations
	 */
	EXPORT Framebuffer(GLenum target = GL_FRAMEBUFFER);
	EXPORT ~Framebuffer();
	/**
	 * @brief Checks the completeness status of the framebuffer
	 * @return Returns the status of the framebuffer
	 */
	EXPORT GLenum get_buffer_status();
	/**
	 * @brief Binds the framebuffer to the target set in the constructor
	 */
	EXPORT void bind();
	/**
	 * @brief Binds the framebuffer to a framebuffer target
	 * @param target The target of the binding operation
	 */
	EXPORT void bind(GLenum target);
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
	/**
	 * @brief Sets the draw buffers to be drawn into
	 */
	EXPORT void finalize();
};

}

#endif
