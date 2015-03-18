#include "texture.h"

using namespace std;

Texture::Texture() {
	glGenTextures(1, &texture);
}

Texture::Texture(Image *image) {
	glGenTextures(1, &texture);
	if(!image) {
		load_texture(image);
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
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
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, image->width, image->height, 0, mode, GL_UNSIGNED_BYTE, image->image->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
