#include "texture.h"

using namespace sgltk;

Texture_3d::Texture_3d() : Texture() {
	target = GL_TEXTURE_3D;
}

Texture_3d::Texture_3d(unsigned int res_x,
		       unsigned int res_y,
		       unsigned int res_z,
		       GLenum internal_format,
		       GLenum type,
		       GLenum format) : Texture() {

	target = GL_TEXTURE_3D;
	create_empty(res_x, res_y, res_z, internal_format, type, format);
}

Texture_3d::Texture_3d(const std::vector<Image>& images) : Texture() {
	target = GL_TEXTURE_3D;
	load(images);
}

Texture_3d::Texture_3d(const std::vector<std::string>& paths) : Texture() {
	target = GL_TEXTURE_3D;
	load(paths);
}

Texture_3d::~Texture_3d() {
}

void Texture_3d::create_empty(unsigned int res_x,
			      unsigned int res_y,
			      unsigned int res_z,
			      GLenum internal_format,
			      GLenum type,
			      GLenum format) {

	width = res_x;
	height = res_y;
	num_layers = res_z;

	bind();
	glTexImage3D(GL_TEXTURE_3D, 0, internal_format,
		     res_x, res_y, res_z, 0, format, type, nullptr);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	unbind();
}

bool Texture_3d::load(const std::vector<std::string>& paths) {
	std::vector<Image> images(paths.size());
	for(unsigned int i = 0; i < paths.size(); i++) {
		if(!images[i].load(paths[i]))
			return false;
	}
	return load(images);
}

bool Texture_3d::load(const std::vector<Image>& images) {
	width = 0;
	height = 0;
	num_layers = images.size();

	std::vector<SDL_Surface *> images_tmp(images.size());

	for(unsigned int i = 0; i < images.size(); i++) {
		if(!images[i].image)
			return false;

		if(width == 0 && height == 0) {
			width = images[i].width;
			height = images[i].height;
		} else if(width != images[i].width || height != images[i].height) {
			return false;
		}

		images_tmp[i] = SDL_ConvertSurfaceFormat(images[i].image,
							 SDL_PIXELFORMAT_RGBA8888, 0);
		if(!images_tmp[i]) {
			return false;
		}
	}

	bind();
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA,
		     width, height, images.size(), 0,
		     GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, nullptr);
	for(unsigned int i = 0; i < num_layers; i++) {
		glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, i,
				width, height, 1, GL_RGBA,
				GL_UNSIGNED_INT_8_8_8_8, images_tmp[i]->pixels);
	}
	glGenerateMipmap(GL_TEXTURE_3D);
	unbind();

	for(SDL_Surface *image : images_tmp) {
		SDL_FreeSurface(image);
	}

	return true;
}

void Texture_3d::bind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_3D, texture);
}

void Texture_3d::unbind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_3D, 0);
}
