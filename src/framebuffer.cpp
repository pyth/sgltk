#include "framebuffer.h"

using namespace sgltk;

int Framebuffer::max_color_attachments = 0;

Framebuffer::Framebuffer(GLenum target) {
	this->target = target;
	width = 0;
	height = 0;
	glGenFramebuffers(1, &buffer);
	if(max_color_attachments == 0)
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS,
			      &Framebuffer::max_color_attachments);
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &buffer);
}

GLenum Framebuffer::get_buffer_status() {
	bind();
	GLenum ret = glCheckFramebufferStatus(target);
	unbind();
	return ret;
}

void Framebuffer::bind() {
	glBindFramebuffer(target, buffer);
}

void Framebuffer::bind(GLenum target) {
	glBindFramebuffer(target, buffer);
	if(draw_buffers.size() == 0) {
		glDrawBuffer(GL_NONE);
	} else {
		glDrawBuffers(draw_buffers.size(), draw_buffers.data());
	}
}

void Framebuffer::unbind() {
	glBindFramebuffer(target, 0);
}

bool Framebuffer::attach_texture(GLenum attachment,
				 Texture& texture) {

	if(width == 0 && height == 0) {
		width = texture.width;
		height = texture.height;
	} else if(width != texture.width || height != texture.height) {
		App::error_string.push_back("The size of the texture does not "
			"match the size of previously attached textures or "
			"renderbuffers.");
		return false;
	}
	bind();
	glFramebufferTexture(target, attachment, texture.texture, 0);
	unbind();
	GLenum attachment_max = GL_COLOR_ATTACHMENT0 + max_color_attachments;
	switch(attachment) {
		case GL_NONE:
		case GL_FRONT_LEFT:
		case GL_FRONT_RIGHT:
		case GL_BACK_LEFT:
		case GL_BACK_RIGHT:
			draw_buffers.push_back(attachment);
			break;
		default:
			if(attachment >= GL_COLOR_ATTACHMENT0 &&
					attachment < attachment_max) {

				draw_buffers.push_back(attachment);
			}
			break;
	}
	return true;
}

bool Framebuffer::attach_texture_layer(GLenum attachment,
				       Texture& texture,
				       unsigned int layer) {

	if(width == 0 && height == 0) {
		width = texture.width;
		height = texture.height;
	} else if(width != texture.width || height != texture.height) {
		App::error_string.push_back("The size of the texture does not "
			"match the size of previously attached textures or "
			"renderbuffers.");
		return false;
	}
	if(layer >= texture.num_layers) {
		App::error_string.push_back("Layer argument is exceeding the"
					    " number of layers of the texture");
		return false;
	}
	bind();
	glFramebufferTextureLayer(target, attachment, texture.texture, 0, layer);
	unbind();
	GLenum attachment_max = GL_COLOR_ATTACHMENT0 + max_color_attachments;
	switch(attachment) {
		case GL_NONE:
		case GL_FRONT_LEFT:
		case GL_FRONT_RIGHT:
		case GL_BACK_LEFT:
		case GL_BACK_RIGHT:
			draw_buffers.push_back(attachment);
			break;
		default:
			if(attachment >= GL_COLOR_ATTACHMENT0 &&
					attachment < attachment_max) {

				draw_buffers.push_back(attachment);
			}
			break;
	}
	return true;
}

bool Framebuffer::attach_renderbuffer(GLenum attachment,
				      Renderbuffer& buffer) {

	if(width == 0 && height == 0) {
		width = buffer.width;
		height = buffer.height;
	} else if(width != buffer.width || height != buffer.height) {
		App::error_string.push_back("The size of the renderbuffer does "
			"not match the size of previously attached textures or "
			"renderbuffers.");
		return false;
	}
	bind();
	glFramebufferRenderbuffer(target,
				  attachment,
				  GL_RENDERBUFFER,
				  buffer.buffer);
	unbind();
	GLenum attachment_max = GL_COLOR_ATTACHMENT0 + max_color_attachments;
	switch(attachment) {
		case GL_NONE:
		case GL_FRONT_LEFT:
		case GL_FRONT_RIGHT:
		case GL_BACK_LEFT:
		case GL_BACK_RIGHT:
			draw_buffers.push_back(attachment);
			break;
		default:
			if(attachment >= GL_COLOR_ATTACHMENT0 &&
					attachment < attachment_max) {

				draw_buffers.push_back(attachment);
			}
			break;
	}
	return true;
}

void Framebuffer::finalize() {
	bind();
	if(draw_buffers.size() == 0) {
		glDrawBuffer(GL_NONE);
	} else {
		glDrawBuffers(draw_buffers.size(), draw_buffers.data());
	}
	unbind();
}

void Framebuffer::blit_to(sgltk::Framebuffer *target,
			  unsigned int src_x0,
			  unsigned int src_y0,
			  unsigned int src_x1,
			  unsigned int src_y1,
			  unsigned int dst_x0,
			  unsigned int dst_y0,
			  unsigned int dst_x1,
			  unsigned int dst_y1,
			  GLbitfield mask,
			  GLenum filter) {

	if(!target) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	} else {
		target->bind(GL_DRAW_FRAMEBUFFER);
	}
	bind(GL_READ_FRAMEBUFFER);
	glBlitFramebuffer(src_x0, src_y0, src_x1, src_y1, dst_x0, dst_y0, dst_x1, dst_y1, mask, filter);
	unbind();
	if(target) {
		target->unbind();
	}
}

void Framebuffer::blit_from_default(unsigned int src_x0,
				    unsigned int src_y0,
				    unsigned int src_x1,
				    unsigned int src_y1,
				    unsigned int dst_x0,
				    unsigned int dst_y0,
				    unsigned int dst_x1,
				    unsigned int dst_y1,
				    GLbitfield mask,
				    GLenum filter) {

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	bind(GL_DRAW_FRAMEBUFFER);
	glBlitFramebuffer(src_x0, src_y0, src_x1, src_y1, dst_x0, dst_y0, dst_x1, dst_y1, mask, filter);
	unbind();
}
