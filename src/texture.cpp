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

void Texture::add_path(std::string path) {
	Image::add_path(path);
}
