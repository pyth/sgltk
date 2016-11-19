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

void Framebuffer::attach_texture(GLenum attachment,
				 Texture& texture) {

	bind();
	glFramebufferTexture(target,
			     attachment,
			     texture.texture, 0);
	unbind();
	if(width == 0 && height == 0) {
		width = texture.width;
		height = texture.height;
	}
	GLenum attachment_max = GL_COLOR_ATTACHMENT0 + max_color_attachments - 1;
	if(attachment >= GL_COLOR_ATTACHMENT0 &&
			attachment <= attachment_max) {
		draw_buffers.push_back(attachment);
	}
}

void Framebuffer::attach_renderbuffer(GLenum attachment,
				      Renderbuffer& buffer) {

	bind();
	glFramebufferRenderbuffer(target,
				  attachment,
				  GL_RENDERBUFFER,
				  buffer.buffer);
	unbind();
	if(width == 0 && height == 0) {
		width = buffer.width;
		height = buffer.height;
	}
	GLenum attachment_max = GL_COLOR_ATTACHMENT0 + max_color_attachments - 1;
	if(attachment >= GL_COLOR_ATTACHMENT0 &&
			attachment <= attachment_max) {
		draw_buffers.push_back(attachment);
	}
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
