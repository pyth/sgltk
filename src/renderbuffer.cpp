#include "renderbuffer.h"

using namespace sgltk;

Renderbuffer::Renderbuffer(unsigned int res_x,
			   unsigned int res_y,
			   GLenum format) {
	this->format = format;
	width = res_x;
	height = res_y;
	glGenRenderbuffers(1, &buffer);
	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, format, res_x, res_y);
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
