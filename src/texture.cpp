#include "texture.h"

using namespace sgltk;

std::map<std::string, Texture *> Texture::textures;

Texture::Texture(GLenum target,
		 unsigned int res_x,
		 unsigned int res_y) {
	this->target = target;
	width = res_x;
	height = res_y;
	glGenTextures(1, &texture);
	glBindTexture(target, texture);
	glTexImage2D(target, 0, GL_RGB, res_x, res_y, 0,
		     GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(GLenum target) {
	this->target = target;
	width = 0;
	height = 0;
	glGenTextures(1, &texture);
}

Texture::Texture(const std::string& path) {
	target = GL_TEXTURE_2D;
	glGenTextures(1, &texture);
	Image img(path);
	width = img.width;
	height = img.height;
	load_texture(img);
}

Texture::Texture(GLenum target, const std::string& path) {
	this->target = target;
	glGenTextures(1, &texture);
	Image img(path);
	width = img.width;
	height = img.height;
	load_texture(img);
}

Texture::Texture(const Image& image) {
	target = GL_TEXTURE_2D;
	width = image.width;
	height = image.height;
	glGenTextures(1, &texture);
	load_texture(image);
}

Texture::Texture(GLenum target, const Image& image) {
	this->target = target;
	width = image.width;
	height = image.height;
	glGenTextures(1, &texture);
	load_texture(image);
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
}

void Texture::set_target(GLenum target) {
	this->target = target;
}

void Texture::bind() {
	bind(0);
}

void Texture::bind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(target, texture);
}

void Texture::unbind() {
	unbind(0);
}

void Texture::unbind(unsigned int texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(target, 0);
}

void Texture::set_parameter(GLenum name, int parameter) {
	glBindTexture(target, texture);
	glTexParameteri(target, name, parameter);
	glBindTexture(target, 0);
}

void Texture::set_parameter(GLenum name, float parameter) {
	glBindTexture(target, texture);
	glTexParameterf(target, name, parameter);
	glBindTexture(target, 0);
}

void Texture::load_texture(const std::string& path) {
	target = GL_TEXTURE_2D;
	glGenTextures(1, &texture);
	Image img(path);
	width = img.width;
	height = img.height;
	load_texture(img);
}

void Texture::load_texture(const Image& image) {
	if(!image.image)
		return;

	SDL_Surface *tmp = SDL_ConvertSurfaceFormat(image.image,
					SDL_PIXELFORMAT_RGBA8888, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(target, texture);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(target, 0, GL_RGBA, image.width, image.height, 0,
		     GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, tmp->pixels);
	glGenerateMipmap(target);
	glBindTexture(target, 0);
	SDL_FreeSurface(tmp);
}

void Texture::load_cubemap(const Image& pos_x, const Image& neg_x,
			   const Image& pos_y, const Image& neg_y,
			   const Image& pos_z, const Image& neg_z) {

	SDL_Surface *tmp;
	std::map<GLenum, const Image *> side_map;
	side_map[GL_TEXTURE_CUBE_MAP_POSITIVE_X] = &pos_x;
	side_map[GL_TEXTURE_CUBE_MAP_NEGATIVE_X] = &neg_x;
	side_map[GL_TEXTURE_CUBE_MAP_POSITIVE_Y] = &pos_y;
	side_map[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y] = &neg_y;
	side_map[GL_TEXTURE_CUBE_MAP_POSITIVE_Z] = &pos_z;
	side_map[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z] = &neg_z;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(target, texture);

	for(const std::pair<GLenum, const Image *>& side : side_map) {
		tmp = SDL_ConvertSurfaceFormat(side.second->image,
					SDL_PIXELFORMAT_RGBA8888, 0);

		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(side.first, 0, GL_RGBA, side.second->width,
			     side.second->height, 0, GL_RGBA,
			     GL_UNSIGNED_INT_8_8_8_8, tmp->pixels);
		SDL_FreeSurface(tmp);
	}
	glBindTexture(target, 0);
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
