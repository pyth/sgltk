#include "framebuffer.h"

using namespace sgltk;

Framebuffer::Framebuffer(GLuint target) {
	this->target = target;
	width = 0;
	height = 0;
	glGenFramebuffers(1, &buffer);
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

void Framebuffer::bind(GLuint target) {
	glBindFramebuffer(target, buffer);
}

void Framebuffer::unbind() {
	glBindFramebuffer(target, 0);
}

void Framebuffer::attach_texture(GLenum attachment,
				 Texture& texture) {

	bind();
	glFramebufferTexture2D(target,
			       attachment,
			       texture.target,
			       texture.texture, 0);
	if(width == 0 && height == 0) {
		width = texture.width;
		height = texture.height;
	}
	unbind();
}

void Framebuffer::attach_renderbuffer(GLenum attachment,
				      Renderbuffer& buffer) {

	bind();
	glFramebufferRenderbuffer(target,
				  attachment,
				  GL_RENDERBUFFER,
				  buffer.buffer);
	if(width == 0 && height == 0) {
		width = buffer.width;
		height = buffer.height;
	}
	unbind();
}
