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

bool Framebuffer::check_buffer_status() {
	if(glCheckFramebufferStatus(target) == GL_FRAMEBUFFER_COMPLETE) {
		return true;
	} else {
		return false;
	}
}

void Framebuffer::bind() {
	glBindFramebuffer(target, buffer);
}

void Framebuffer::unbind() {
	glBindFramebuffer(target, 0);
}

void Framebuffer::attach_texture(GLenum attachment,
				 Texture& texture) {

	glFramebufferTexture2D(target,
			       attachment,
			       texture.target,
			       texture.texture, 0);
	/*GLenum status = glCheckFramebufferStatus(target);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE:
			std::cout<<"framebuffer complete\n";
			break;
		case GL_FRAMEBUFFER_UNDEFINED:
			std::cout<<"framebuffer undefined\n";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout<<"framebuffer incomplete attachment\n";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout<<"framebuffer incomplete missing attachment\n";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cout<<"framebuffer incomplete draw buffer\n";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cout<<"framebuffer incomplete read buffer\n";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout<<"framebuffer unsupported\n";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			std::cout<<"framebuffer incomplete multisample\n";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			std::cout<<"framebuffer incomplete layer targets\n";
			break;
	}*/
	if(width == 0 && height == 0) {
		width = texture.width;
		height = texture.height;
	}
}

void Framebuffer::attach_renderbuffer(GLenum attachment,
				      Renderbuffer& buffer) {

	glFramebufferRenderbuffer(target,
				  attachment,
				  GL_RENDERBUFFER,
				  buffer.buffer);
	switch(attachment) {
		case GL_DEPTH_ATTACHMENT:
			depth_attachment = true;
			break;
		case GL_STENCIL_ATTACHMENT:
			stencil_attachment = true;
			break;
	}
	if(width == 0 && height == 0) {
		width = buffer.width;
		height = buffer.height;
	}
}
