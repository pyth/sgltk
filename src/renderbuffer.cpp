#include "renderbuffer.h"

using namespace sgltk;

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
