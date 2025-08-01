#include <sgltk/texture.h>

using namespace sgltk;

Texture_1d::Texture_1d() : Texture() {
	target = GL_TEXTURE_1D;
}

Texture_1d::Texture_1d(unsigned int res, GLenum internal_format, GLenum type, GLenum format) : Texture() {
	target = GL_TEXTURE_1D;
	create_empty(res, internal_format, type, format);
}

Texture_1d::Texture_1d(const Image& image) : Texture() {
	target = GL_TEXTURE_1D;
	load(image);
}

Texture_1d::Texture_1d(const std::string& path) : Texture() {
	target = GL_TEXTURE_1D;
	load(path);
}

Texture_1d::~Texture_1d() {
}

void Texture_1d::create_empty(unsigned int res,
			      GLenum internal_format,
			      GLenum type, GLenum format) {
	width = res;
	height = 1;

	bind();
	glTexImage1D(GL_TEXTURE_1D, 0,
		     internal_format, res, 0, format, type, nullptr);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	unbind();
}

bool Texture_1d::load(const std::string& path) {
	Image img;
	if(!img.load(path))
		return false;

	width = img.width;
	height = 1;
	return load(img);
}

bool Texture_1d::load(const Image& image) {
	if(!image.image)
		return false;

	SDL_Surface *tmp = SDL_ConvertSurfaceFormat(image.image,
		SDL_PIXELFORMAT_RGBA8888, 0);
	if(!tmp) {
		return false;
	}

	bind();
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, image.width, 0,
		     GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, tmp->pixels);
	glGenerateMipmap(GL_TEXTURE_1D);
	unbind();
	SDL_FreeSurface(tmp);
	return true;
}

void Texture_1d::bind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_1D, texture);
}

void Texture_1d::unbind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_1D, 0);
}
