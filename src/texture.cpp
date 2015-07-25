#include "texture.h"

std::map<char *, Texture *> Texture::textures;

Texture::Texture() {
	init_lib();
	target = GL_TEXTURE_2D;
	glGenTextures(1, &texture);
}

Texture::Texture(GLenum target) {
	init_lib();
	target = GL_TEXTURE_2D;
	glGenTextures(1, &texture);
}

Texture::Texture(const char *path) {
	init_lib();
	target = GL_TEXTURE_2D;
	glGenTextures(1, &texture);
	if(path) {
		Image img(path);
		load_texture(&img);
	}
}

Texture::Texture(GLenum target, const char *path) {
	init_lib();
	this->target = target;
	glGenTextures(1, &texture);
	if(path) {
		Image img(path);
		load_texture(&img);
	}
}

Texture::Texture(Image *image) {
	init_lib();
	target = GL_TEXTURE_2D;
	glGenTextures(1, &texture);
	if(image) {
		load_texture(image);
	}
}

Texture::Texture(GLenum target, Image *image) {
	init_lib();
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
	glTexParameterf(target, name, parameter);
	glBindTexture(target, 0);
}

void Texture::load_texture(Image *image) {
	if(!image || !(image->image)) {
		return;
	}

	int mode;
	switch(image->image->format->BytesPerPixel) {
	case 3:
		mode = GL_RGB;
		break;
	case 4:
		mode = GL_RGBA;
		break;
	default:
		return;
		break;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(target, texture);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(target, 0, mode, image->width, image->height, 0,
		     mode, GL_UNSIGNED_BYTE, image->image->pixels);
	glGenerateMipmap(target);
	glBindTexture(target, 0);
}

bool Texture::store_texture(const char *name, Texture *texture) {
	return textures.insert(std::make_pair((char *)name, texture)).second;
}

Texture *Texture::find_texture(const char *name) {
	std::map<char *, Texture *>::iterator it = textures.find((char *)name);
	if(it == textures.end())
		return NULL;
	return it->second;
}
