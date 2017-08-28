#include "texture.h"

using namespace sgltk;

Texture_1d_Array::Texture_1d_Array() : Texture() {
	target = GL_TEXTURE_1D_ARRAY;
}

Texture_1d_Array::Texture_1d_Array(unsigned int res,
				   unsigned int num_layers,
				   GLenum internal_format,
				   GLenum type,
				   GLenum format) : Texture() {

	target = GL_TEXTURE_1D_ARRAY;
	create_empty(res, num_layers, internal_format, type, format);
}

Texture_1d_Array::Texture_1d_Array(const std::vector<Image>& images) : Texture() {
	target = GL_TEXTURE_1D_ARRAY;
	load(images);
}

Texture_1d_Array::Texture_1d_Array(const std::vector<std::string>& paths) : Texture() {
	target = GL_TEXTURE_1D_ARRAY;
	load(paths);
}

Texture_1d_Array::~Texture_1d_Array() {
}

void Texture_1d_Array::create_empty(unsigned int res,
				    unsigned int num_layers,
				    GLenum internal_format,
				    GLenum type, GLenum format) {

	width = res;
	height = 1;
	this->num_layers = num_layers;

	bind();
	glTexImage2D(GL_TEXTURE_1D_ARRAY, 0, internal_format,
		     res, num_layers, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	unbind();
}

bool Texture_1d_Array::load(const std::vector<std::string>& paths) {
	std::vector<Image> images(paths.size());
	for(unsigned int i = 0; i < paths.size(); i++) {
		if(!images[i].load(paths[i]))
			return false;
	}
	return load(images);
}

bool Texture_1d_Array::load(const std::vector<Image>& images) {
	width = 0;
	height = 1;
	num_layers = images.size();

	std::vector<SDL_Surface *> images_tmp(images.size());

	for(unsigned int i = 0; i < images.size(); i++) {
		if(!images[i].image)
			return false;

		if(width == 0) {
			width = images[i].width;
		} else if(width != images[i].width) {
			return false;
		}

		images_tmp[i] = SDL_ConvertSurfaceFormat(images[i].image,
							 SDL_PIXELFORMAT_RGBA8888, 0);
		if(!images_tmp[i]) {
			return false;
		}
	}

	bind();
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_1D_ARRAY, 0, GL_RGBA,
		     width, images.size(), 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, NULL);
	for(unsigned int i = 0; num_layers; i++) {
		glTexSubImage2D(GL_TEXTURE_1D_ARRAY, 0, 0, i, width, 1,
				GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, images_tmp[i]->pixels);
	}
	glGenerateMipmap(GL_TEXTURE_1D_ARRAY);
	unbind();

	for(SDL_Surface *image : images_tmp) {
		SDL_FreeSurface(image);
	}

	return true;
}

void Texture_1d_Array::bind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_1D_ARRAY, texture);
}

void Texture_1d_Array::unbind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_1D_ARRAY, 0);
}
