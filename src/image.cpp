#include "texture.h"

using namespace std;

Image::Image() {
	init_lib();
	path = NULL;
	image = NULL;
	width = 0;
	height = 0;
}

Image::Image(const char *filename) {
	init_lib();
	load(filename);
}

Image::~Image() {
	delete[] path;
	SDL_FreeSurface(image);
}

bool Image::create_empty(int width, int height) {
	if(image) {
		SDL_FreeSurface(image);
		image = NULL;
	}

	this->width = width;
	this->height = height;

	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	image = SDL_CreateRGBSurface(0, width, height, 32,
				     rmask, gmask, bmask, amask);
	if(!image) {
		return false;
	}
	return true;
}

bool Image::load(const char *filename) {
	if(!filename) {
		path = NULL;
		width = 0;
		height = 0;
		return false;
	}

	if(image)
		SDL_FreeSurface(image);

	image = IMG_Load(filename);
	if(!image) {
		cerr << "Unable to load image: " << filename << " - "
		     << IMG_GetError() << endl;
		path = NULL;
		width = 0;
		height = 0;
		return false;
	}

	size_t len = strlen(filename);
	path = new char[len + 1];
	strncpy(path, filename, len);
	width = image->w;
	height = image->h;

	return true;
}

bool Image::create_text(const char *text, TTF_Font *font, int size,
			Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	if(image) {
		SDL_FreeSurface(image);
		image = NULL;
	}
	SDL_Color color = {r, g, b, a};
	image = TTF_RenderText_Blended(font, text, color);
	if(!image) {
		cerr << "TTF_RenderText_Blended failed." << endl;
		return false;
	}
	width = image->w;
	height = image->h;
	return true;
}

bool Image::create_text(const char *text, const char *font_file, int size,
			Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	TTF_Font *font = TTF_OpenFont(font_file, size);
	if(!font) {
		cerr << "TTF_OpenFont for " << font_file << " failed." << endl;
		return false;
	}

	bool ret = create_text(text, font, size, r, g, b, a);
	TTF_CloseFont(font);

	return ret;
}

bool Image::copy_from(const Image *src, int x, int y) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	return copy_from(src, &rect);
}

bool Image::copy_from(const Image *src, SDL_Rect *dst_rect) {
	return copy_from(src, dst_rect, NULL);
}

bool Image::copy_from(const Image *src, SDL_Rect *dst_rect,
		      SDL_Rect *src_rect) {
	if(!src) {
		return false;
	}

	if(!image) {
		image = SDL_ConvertSurface(src->image, src->image->format,
					   src->image->flags);
		if(image) {
			return false;
		}
	}
	if(SDL_BlitSurface(src->image, src_rect, image, dst_rect) < 0) {
		SDL_FreeSurface(image);
		return false;
	}
	return true;
}

void Image::set_color_key(int r, int g, int b) {
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, r, g, b));
}
