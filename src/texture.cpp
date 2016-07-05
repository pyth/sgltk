#include "texture.h"

using namespace sgltk;

std::map<std::string, Texture *> Texture::textures;

Texture::Texture() {
	target = GL_TEXTURE_2D;
	glGenTextures(1, &texture);
}

Texture::Texture(GLenum target) {
	target = GL_TEXTURE_2D;
	glGenTextures(1, &texture);
}

Texture::Texture(std::string path) {
	target = GL_TEXTURE_2D;
	glGenTextures(1, &texture);
	Image img(path);
	load_texture(&img);
}

Texture::Texture(GLenum target, std::string path) {
	this->target = target;
	glGenTextures(1, &texture);
	Image img(path);
	load_texture(&img);
}

Texture::Texture(Image *image) {
	target = GL_TEXTURE_2D;
	glGenTextures(1, &texture);
	if(image) {
		load_texture(image);
	}
}

Texture::Texture(GLenum target, Image *image) {
	this->target = target;
	glGenTextures(1, &texture);
	if(image) {
		load_texture(image);
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
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
	glTexParameteri(target, name, parameter);
	glBindTexture(target, 0);
}

void Texture::load_texture(Image *image) {
	if(!image || !(image->image)) {
		return;
	}

	SDL_Surface *tmp = SDL_ConvertSurfaceFormat(image->image, SDL_PIXELFORMAT_RGBA8888, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(target, texture);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(target, 0, GL_RGBA, image->width, image->height, 0,
		     GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, tmp->pixels);
	glGenerateMipmap(target);
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
