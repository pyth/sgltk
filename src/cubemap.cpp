#include <sgltk/texture.h>

using namespace sgltk;

Cubemap::Cubemap() : Texture() {
	target = GL_TEXTURE_CUBE_MAP;
}

Cubemap::Cubemap(unsigned int res_x,
		 unsigned int res_y,
		 GLenum internal_format,
		 GLenum type, GLenum format) : Texture() {

	target = GL_TEXTURE_CUBE_MAP;
	create_empty(res_x, res_y, internal_format, type, format);
}

Cubemap::Cubemap(const Image& pos_x, const Image& neg_x,
		 const Image& pos_y, const Image& neg_y,
		 const Image& pos_z, const Image& neg_z) : Texture() {

	target = GL_TEXTURE_CUBE_MAP;
	load(pos_x, neg_x, pos_y, neg_y, pos_z, neg_z);
}

Cubemap::~Cubemap() {
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
			type, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unbind();
}

bool Cubemap::load(const std::string& pos_x, const std::string& neg_x,
		   const std::string& pos_y, const std::string& neg_y,
		   const std::string& pos_z, const std::string& neg_z) {

	Image positive_x(pos_x);
	Image negative_x(neg_x);
	Image positive_y(pos_y);
	Image negative_y(neg_y);
	Image positive_z(pos_z);
	Image negative_z(neg_z);

	return load(positive_x, negative_x, positive_y, negative_y, positive_z, negative_z);
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
