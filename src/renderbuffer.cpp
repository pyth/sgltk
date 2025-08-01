#include <sgltk/renderbuffer.h>

using namespace sgltk;

Renderbuffer::Renderbuffer() {
	this->format = GL_FLOAT;
	this->width = 100;
	this->height = 100;
	glGenRenderbuffers(1, &buffer);
	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	unbind();
}

Renderbuffer::Renderbuffer(unsigned int width,
			   unsigned int height,
			   GLenum format) {
	this->format = format;
	this->width = width;
	this->height = height;
	glGenRenderbuffers(1, &buffer);
	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	unbind();
}

Renderbuffer::~Renderbuffer() {
	glDeleteRenderbuffers(1, &buffer);
}

void Renderbuffer::bind() {
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
}

void Renderbuffer::unbind() {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::set_format(GLenum format) {
	this->format = format;
	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	unbind();
}

void Renderbuffer::set_size(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	unbind();
}
