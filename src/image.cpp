#include "image.h"

using namespace sgltk;

std::vector<std::string> Image::paths = {"./"}; 

Image::Image() {
	image = NULL;
	width = 0;
	height = 0;
	image = NULL;
}

Image::Image(std::string filename) {
	image = NULL;
	width = 0;
	height = 0;
	if(!load(filename)) {
		std::string error = std::string("Error loading image: ") +
			SDL_GetError();
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}
}

Image::~Image() {
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

bool Image::load(std::string filename) {
	width = 0;
	height = 0;

	if(image)
		SDL_FreeSurface(image);

	if((filename.length() > 1 && filename[0] == '/') ||
			(filename.length() > 2 && filename[1] == ':')) {
		//absolute path
		image = IMG_Load(filename.c_str());
	} else {
		//relative path
		for(unsigned int i = 0; i < paths.size(); i++) {
			image = IMG_Load((paths[i] + filename).c_str());
			if(image)
				break;
		}
	}
	if(!image) {
		App::error_string.push_back(std::string("Unable to load image: ")
			+ filename + std::string(" - ") + IMG_GetError());
		width = 0;
		height = 0;
		return false;
	}

	width = image->w;
	height = image->h;

	return true;
}

#ifdef HAVE_SDL_TTF_H
bool Image::create_text(std::string text, TTF_Font *font, int size,
			Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	if (!font)
		return false;
	if(image) {
		SDL_FreeSurface(image);
		image = NULL;
	}
	SDL_Color color = {r, g, b, a};
	image = TTF_RenderText_Blended(font, text.c_str(), color);
	if(!image) {
		App::error_string.push_back(std::string("TTF_RenderText_Blended failed."));
		return false;
	}
	width = image->w;
	height = image->h;
	return true;
}

bool Image::create_text(std::string text, std::string font_file, int size,
			Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	TTF_Font *font;
	if((font_file.length() > 1 && font_file[0] == '/') ||
			(font_file.length() > 2 && font_file[1] == ':')) {
		//absolute path
		font = TTF_OpenFont(font_file.c_str(), size);
	} else {
		//relative path
		for(unsigned int i = 0; i < paths.size(); i++) {
			font = TTF_OpenFont((paths[i] + font_file).c_str(), size);
			if(font)
				break;
		}
	}
	if(!font) {
		App::error_string.push_back(std::string("TTF_OpenFont for ")
				+ font_file + std::string(" failed."));
		return false;
	}

	bool ret = create_text(text, font, size, r, g, b, a);
	TTF_CloseFont(font);
	return ret;
}
#endif //HAVE_SDL_TTF_H

bool Image::copy_from(const Image *src, int x, int y) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	return copy_from(src, &rect, NULL);
}

bool Image::copy_from(const Image *src, SDL_Rect *dst_rect) {
	return copy_from(src, dst_rect, NULL);
}

bool Image::copy_from(const Image *src, SDL_Rect *dst_rect,
		      SDL_Rect *src_rect) {
	if(!src || !src->image) {
		return false;
	}

	if(!image) {
		image = SDL_ConvertSurface(src->image, src->image->format,
					   src->image->flags);
		if(!image) {
			return false;
		}
	}
	if(SDL_BlitSurface(src->image, src_rect, image, dst_rect) < 0) {
		SDL_FreeSurface(image);
		return false;
	}
	return true;
}

/*bool Image::copy_scaled() {
	SDL_BlitScaled(src,srcrect, dst, dstrect);
}*/

void Image::set_color_key(int r, int g, int b) {
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, r, g, b));
}

void Image::add_path(std::string path) {
	if(path[path.length() - 1] != '/')
		path += '/';

	if(std::find(paths.begin(), paths.end(), path) == paths.end())
		paths.push_back(path);
}
