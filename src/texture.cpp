#include "texture.h"

using namespace sgltk;

std::map<std::string, std::shared_ptr<Texture> > Texture::textures;

Texture::Texture() {
	width = 0;
	height = 0;
	num_layers = 1;
	glGenTextures(1, &texture);
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
}

void Texture::set_parameter(GLenum name, int parameter) {
	bind();
	glTexParameteri(target, name, parameter);
	unbind();
}

void Texture::set_parameter(GLenum name, float parameter) {
	bind();
	glTexParameterf(target, name, parameter);
	unbind();
}

void Texture::set_parameter(GLenum name, float *parameter) {
	bind();
	glTexParameterfv(target, name, parameter);
	unbind();
}

bool Texture::store_texture(std::string name, std::shared_ptr<Texture> texture) {
	return textures.insert(std::make_pair(name, texture)).second;
}

std::shared_ptr<Texture> Texture::find_texture(std::string name) {
	const auto& it = textures.find(name);
	if(it == textures.end())
		return nullptr;
	return it->second;
}

void Texture::add_path(std::string path) {
	Image::add_path(path);
}

void Texture::bind_image(GLuint unit, GLint level,
			 GLboolean layered, GLint layer,
			 GLenum access, GLenum format) {

	glBindImageTexture(unit, texture, level, layered, layer, access, format);
}
