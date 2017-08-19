#include "texture.h"

using namespace sgltk;

Texture_3d::Texture_3d() : Texture() {
	target = GL_TEXTURE_3D;
}

Texture_3d::Texture_3d(unsigned int res_x, unsigned int res_y, unsigned int res_z, GLenum internal_format, GLenum type, GLenum format) : Texture() {
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

void Texture_3d::create_empty(unsigned int res_x, unsigned int res_y, unsigned int res_z, GLenum internal_format, GLenum type, GLenum format) {
	width = res_x;
	height = res_y;

	bind();
	glTexImage3D(GL_TEXTURE_3D, 0, internal_format, res_x, res_y, res_z, 0,
		format, type, NULL);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	unbind();
}

bool Texture_3d::load(const std::vector<std::string>& paths) {
	std::vector<Image> images;
	for(std::string path : paths) {
		Image img;
		try {
			img.load(path);
		}
		catch(std::exception e) {
			return false;
		}
		images.push_back(img);
	}
	return load(images);
}

bool Texture_3d::load(const std::vector<Image>& images) {
	unsigned int width = 0;
	unsigned int height = 0;

	std::vector<void *> data;
	std::vector<SDL_Surface *> images_tmp;

	for(Image image : images) {
		if(!image.image)
			return false;

		if(width == 0 && height == 0) {
			width = image.width;
			height = image.height;
		}

		SDL_Surface *tmp = SDL_ConvertSurfaceFormat(image.image,
			SDL_PIXELFORMAT_RGBA8888, 0);
		if(!tmp) {
			return false;
		}
		data.push_back(tmp->pixels);
		images_tmp.push_back(tmp);
	}
	unsigned int depth = images_tmp.size();

	bind();
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0,
		GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data.data());
	glGenerateMipmap(GL_TEXTURE_3D);
	unbind();

	for(SDL_Surface *image : images_tmp) {
		SDL_FreeSurface(image);
	}

	return true;
}

void Texture_3d::set_parameter(GLenum name, int parameter) {
	bind();
	glTexParameteri(GL_TEXTURE_3D, name, parameter);
	unbind();
}

void Texture_3d::set_parameter(GLenum name, float parameter) {
	bind();
	glTexParameterf(GL_TEXTURE_3D, name, parameter);
	unbind();
}

void Texture_3d::set_parameter(GLenum name, float *parameter) {
	bind();
	glTexParameterfv(GL_TEXTURE_3D, name, parameter);
	unbind();
}

void Texture_3d::bind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_3D, texture);
}

void Texture_3d::unbind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_3D, 0);
}
