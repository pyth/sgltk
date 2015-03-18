#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "core.h"

using namespace std;

class Image {
public:
	int width;
	int height;

	SDL_Surface *image;

	Image();
	Image(const char *filename);
	~Image();

	bool create_empty(int width, int height);

	bool load(const char *filename);

	bool create_text(const char *text, TTF_Font *font, int size, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	bool create_text(const char *text, const char *font_file, int size, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	bool copy_from(const Image *src, int x, int y);
	bool copy_from(const Image *src, SDL_Rect *dst_rect);
	bool copy_from(const Image *src, SDL_Rect *dst_rect, SDL_Rect *src_rect);

	void set_color_key(int r, int g, int b);
};

class Texture {
public:
	GLuint texture;
	Texture();
	Texture(Image *image);
	~Texture();

	void load_texture(Image *image);
	void bind();
	void unbind();
};

#endif
