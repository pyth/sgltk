#include "texture.h"

using namespace sgltk;

std::map<std::string, Texture *> Texture::textures;

Texture::Texture() {
	glGenTextures(1, &texture);
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
}

bool Texture::store_texture(std::string name, Texture *texture) {
	return textures.insert(std::make_pair(name, texture)).second;
}

Texture *Texture::find_texture(std::string name) {
	std::map<std::string, Texture *>::iterator it = textures.find(name);
	if(it == textures.end())
		return NULL;
	return it->second;
}


Texture_2d::Texture_2d() : Texture() {
}

Texture_2d::Texture_2d(unsigned int res_x, unsigned int res_y, GLenum internal_format, GLenum type, GLenum format) : Texture() {

	create_empty(res_x, res_y, internal_format, type, format);
}

Texture_2d::Texture_2d(const Image& image) : Texture() {
	load(image);
}

Texture_2d::Texture_2d(const std::string& path) : Texture() {
	Image img(path);
	width = img.width;
	height = img.height;
	load(img);
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


Cubemap::Cubemap() : Texture() {
}

Cubemap::Cubemap(unsigned int res_x,
		 unsigned int res_y,
		 GLenum internal_format,
		 GLenum type, GLenum format) : Texture() {

	create_empty(res_x, res_y, internal_format, type, format);
}

Cubemap::Cubemap(const Image& pos_x, const Image& neg_x,
		 const Image& pos_y, const Image& neg_y,
		 const Image& pos_z, const Image& neg_z) : Texture() {

	load(pos_x, neg_x, pos_y, neg_y, pos_z, neg_z);
}

Cubemap::~Cubemap() {
}

void Cubemap::set_parameter(GLenum name, int parameter) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, name, parameter);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::set_parameter(GLenum name, float parameter) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, name, parameter);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::create_empty(unsigned int res_x,
			   unsigned int res_y,
			   GLenum internal_format,
			   GLenum type, GLenum format) {

	width = res_x;
	height = res_y;
	bind();

	for(unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			internal_format, res_x, res_y, 0, format,
			type, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unbind();
}

bool Cubemap::load(const Image& pos_x, const Image& neg_x,
		   const Image& pos_y, const Image& neg_y,
		   const Image& pos_z, const Image& neg_z) {

	SDL_Surface *tmp;
	std::vector<const Image *> sides =
		{&pos_x, &neg_x, &pos_y, &neg_y, &pos_z, &neg_z};

	bind();
	for(unsigned int i = 0; i < 6; i++) {
		tmp = SDL_ConvertSurfaceFormat(sides[i]->image,
			SDL_PIXELFORMAT_RGBA8888, 0);

		if(!tmp)
			return false;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
			sides[i]->width, sides[i]->height,
			0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, tmp->pixels);
		SDL_FreeSurface(tmp);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unbind();
	return true;
}

void Cubemap::bind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

void Cubemap::unbind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}