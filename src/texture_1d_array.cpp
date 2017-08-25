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

	bind();
	glTexImage2D(GL_TEXTURE_1D_ARRAY, 0, internal_format,
		     res, num_layers, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	unbind();
}

bool Texture_1d_Array::load(const std::vector<std::string>& paths) {
	std::vector<Image> images;
	for(std::string path : paths) {
		Image img;
		if(!img.load(path))
			return false;

		images.push_back(img);
	}
	return load(images);
}

bool Texture_1d_Array::load(const std::vector<Image>& images) {
	width = 0;
	height = 1;

	std::vector<void *> data;
	std::vector<SDL_Surface *> images_tmp;

	for(Image image : images) {
		if(!image.image)
			return false;

		if(width == 0) {
			width = image.width;
		} else if(width != image.width) {
			return false;
		}

		SDL_Surface *tmp = SDL_ConvertSurfaceFormat(image.image,
			SDL_PIXELFORMAT_RGBA8888, 0);
		if(!tmp) {
			return false;
		}
		data.push_back(tmp->pixels);
		images_tmp.push_back(tmp);
	}

	bind();
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	for(unsigned int i = 0; data.size(); i++) {
		glTexSubImage2D(GL_TEXTURE_1D_ARRAY, 0, 0, i, width, 1,
				GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, data[i]);
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
