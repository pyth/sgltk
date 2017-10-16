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
	 * @return Returns true on success, false otherwise
	 */
	EXPORT bool attach_texture(GLenum attachment,
				   Texture& texture);
	/**
	 * @brief Attaches the layer of a texture to the framebuffer
	 * @param attachment The attachment point to which the texture layer
	 * 	is attached
	 * @param texture The texture containing the layer to attach
	 * @param layer The layer to attach
	 * @return Returns true on success, false otherwise
	 */
	EXPORT bool attach_texture_layer(GLenum attachment,
					 Texture& texture,
				 	 unsigned int layer);
	/**
	 * @brief Attaches a renderbuffer to the framebuffer
	 * @param attachment The attachment point to which
	 * 	  the renderbuffer is attached
	 * @param buffer The renderbuffer to attach
	 * @return Returns true on success, false otherwise
	 */
	EXPORT bool attach_renderbuffer(GLenum attachment,
					Renderbuffer& buffer);
	/**
	 * @brief Sets the draw buffers to be drawn into
	 */
	EXPORT void finalize();
	/**
	 * @brief Copies a block of pixels from this framebuffer to another one
	 * @param target The framebuffer to copy to. A null pointer indicates
	 * 	the default framebuffer
	 * @param src_x0,src_y0,src_x1,src_y1 The bounds of the source rectangle
	 * 	within the default framebuffer
	 * @param dst_x0,dst_y0,dst_x1,dst_y1 The bounds of the destination
	 * 	rectangle
	 * @param mask The bitwise OR of the flags indicating which buffers are
	 * 	to be copied. The allowed flags are GL_COLOR_BUFFER_BIT,
	 * 	GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT.
	 * @param filter The interpolation to be applied if the image is
	 * 	stretched. Must be GL_NEAREST or GL_LINEAR.
	 */
	EXPORT void blit_to(sgltk::Framebuffer *target,
			    unsigned int src_x0,
			    unsigned int src_y0,
			    unsigned int src_x1,
			    unsigned int src_y1,
			    unsigned int dst_x0,
			    unsigned int dst_y0,
			    unsigned int dst_x1,
			    unsigned int dst_y1,
			    GLbitfield mask,
			    GLenum filter);
	/**
	 * @brief Copies a block of pixels from the default framebuffer to this one
	 * @param src_x0,src_y0,src_x1,src_y1 The bounds of the source rectangle
	 * 	within the default framebuffer
	 * @param dst_x0,dst_y0,dst_x1,dst_y1 The bounds of the destination
	 * 	rectangle
	 * @param mask The bitwise OR of the flags indicating which buffers are
	 * 	to be copied. The allowed flags are GL_COLOR_BUFFER_BIT,
	 * 	GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT.
	 * @param filter The interpolation to be applied if the image is
	 * 	stretched. Must be GL_NEAREST or GL_LINEAR.
	 */
	EXPORT void blit_from_default(unsigned int src_x0,
				      unsigned int src_y0,
				      unsigned int src_x1,
				      unsigned int src_y1,
				      unsigned int dst_x0,
				      unsigned int dst_y0,
				      unsigned int dst_x1,
				      unsigned int dst_y1,
				      GLbitfield mask,
				      GLenum filter);
};

}

#endif
