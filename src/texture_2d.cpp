#include "texture.h"

using namespace sgltk;

Texture_2d::Texture_2d() : Texture() {
}

Texture_2d::Texture_2d(unsigned int res_x, unsigned int res_y, GLenum internal_format, GLenum type, GLenum format) : Texture() {

	create_empty(res_x, res_y, internal_format, type, format);
}

Texture_2d::Texture_2d(const Image& image) : Texture() {
	load(image);
}

Texture_2d::Texture_2d(const std::string& path) : Texture() {
	load(path);
}

Texture_2d::~Texture_2d() {
}

void Texture_2d::create_empty(unsigned int res_x, unsigned int res_y, GLenum internal_format, GLenum type, GLenum format) {
	width = res_x;
	height = res_y;

	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, res_x, res_y, 0,
		format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	unbind();
}

bool Texture_2d::load(const std::string& path) {
	Image img;
	try {
		img.load(path);
	}
	catch(std::exception e) {
		return false;
	}
	width = img.width;
	height = img.height;
	return load(img);
}

bool Texture_2d::load(const Image& image) {
	if(!image.image)
		return false;

	SDL_Surface *tmp = SDL_ConvertSurfaceFormat(image.image,
		SDL_PIXELFORMAT_RGBA8888, 0);
	if(!tmp) {
		return false;
	}

	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
		GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, tmp->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	unbind();
	SDL_FreeSurface(tmp);
	return true;
}

void Texture_2d::set_parameter(GLenum name, int parameter) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, name, parameter);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture_2d::set_parameter(GLenum name, float parameter) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, name, parameter);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture_2d::bind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture_2d::unbind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}