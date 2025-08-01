#include <sgltk/image.h>

using namespace sgltk;

std::vector<std::string> Image::paths = {"./"};

Image::Image() {
	free_data = false;
	image = nullptr;
	width = 0;
	height = 0;
	bytes_per_pixel = 0;
	data = nullptr;
}

Image::Image(std::string filename) {
	free_data = false;
	image = nullptr;
	width = 0;
	height = 0;
	bytes_per_pixel = 0;
	data = nullptr;
	if(!load(filename)) {
		std::string error = std::string("Error loading image: ") +
			SDL_GetError();
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}
}

Image::Image(unsigned int width,
	     unsigned int height,
	     unsigned int bytes_per_pixel,
	     void *data) {

	free_data = false;
	image = nullptr;
	width = 0;
	height = 0;
	bytes_per_pixel = 0;
	data = nullptr;
	if (!load(width, height, bytes_per_pixel, data)) {
		std::string error = std::string("Error loading image: ") +
			SDL_GetError();
		App::error_string.push_back(error);
		throw std::runtime_error(error);
	}
}

Image::~Image() {
	SDL_FreeSurface(image);
	if(free_data)
		free(data);
}

bool Image::create_empty(unsigned int width, unsigned int height) {
	if(image) {
		SDL_FreeSurface(image);
		if(free_data)
			free(data);
		image = nullptr;
	}
	free_data = false;

	this->width = width;
	this->height = height;
	bytes_per_pixel = 4;

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
		App::error_string.push_back(std::string("Unable to create an"
			"empty image: ") + SDL_GetError());
		this->width = 0;
		this->height = 0;
		bytes_per_pixel = 0;
		data = nullptr;
		return false;
	}
	data = image->pixels;
	return true;
}

bool Image::load(const std::string& filename) {
	if(image) {
		SDL_FreeSurface(image);
		if(free_data)
			free(data);
	}
	free_data = false;

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
		bytes_per_pixel = 0;
		data = nullptr;
		return false;
	}

	width = image->w;
	height = image->h;
	bytes_per_pixel = image->format->BytesPerPixel;
	data = image->pixels;

	return true;
}

bool Image::load(unsigned int width,
		 unsigned int height,
		 unsigned int bytes_per_pixel,
		 void *data) {

	if(image) {
		SDL_FreeSurface(image);
		if(free_data)
			free(this->data);
		image = nullptr;
	}

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

	if(!data) {
		App::error_string.push_back(std::string("Image - load: "
			"data is null"));
		this->width = 0;
		this->height = 0;
		this->bytes_per_pixel = 0;
		this->data = nullptr;
		free_data = false;
		return false;
	}

	this->width = width;
	this->height = height;
	this->bytes_per_pixel = bytes_per_pixel;
	this->data = malloc(width * height * bytes_per_pixel);
	if(!this->data) {
		App::error_string.push_back(std::string("Image - load: "
			"Unable to allocate memory"));
		this->width = 0;
		this->height = 0;
		this->bytes_per_pixel = 0;
		free_data = false;
		return false;
	}
	memcpy(this->data, data, width * height * bytes_per_pixel);
	free_data = true;

	image = SDL_CreateRGBSurfaceFrom(this->data, width, height,
			8 * bytes_per_pixel, bytes_per_pixel * width,
			rmask, gmask, bmask, amask);

	if(!image) {
		App::error_string.push_back(std::string("Unable to load the"
			"image from buffer: ") + SDL_GetError());
		free(this->data);
		this->data = nullptr;
		this->width = 0;
		this->height = 0;
		this->bytes_per_pixel = 0;
		free_data = false;
		return false;
	}
	return true;
}

bool Image::save(const std::string& filename) {
	int ret = SDL_SaveBMP(image, filename.c_str());
	if(ret < 0) {
		App::error_string.push_back(std::string("Unable to save the"
			"image to ") + filename + std::string(": ") + SDL_GetError());
		return false;
	}
	return true;
}

#ifdef HAVE_SDL_TTF_H
TTF_Font *Image::open_font_file(const std::string& font_file, unsigned int size) {
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
	return font;
}

void Image::close_font_file(TTF_Font *font_file) {
	TTF_CloseFont(font_file);
}

bool Image::create_text(const std::string& text,
			TTF_Font *font,	Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	if (!font)
		return false;
	if(image) {
		SDL_FreeSurface(image);
		if(free_data)
			free(data);
		image = nullptr;
	}
	SDL_Color color = {r, g, b, a};
	image = TTF_RenderUTF8_Blended(font, text.c_str(), color);
	if(!image) {
		App::error_string.push_back(
			std::string("TTF_RenderUTF8_Blended failed: ") +
			TTF_GetError());
		return false;
	}
	width = image->w;
	height = image->h;
	bytes_per_pixel = 4;
	data = image->pixels;
	free_data = false;
	return true;
}

bool Image::create_text(const std::string& text, const std::string& font_file,
			unsigned int size, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	TTF_Font *font = open_font_file(font_file, size);
	if(!font) {
		App::error_string.push_back(std::string("TTF_OpenFont for ")
				+ font_file + std::string(" failed."));
		return false;
	}

	bool ret = create_text(text, font, r, g, b, a);
	if(ret) {
		data = image->pixels;
		free_data = false;
	}
	close_font_file(font);
	return ret;
}
#endif //HAVE_SDL_TTF_H

bool Image::copy_from(const Image& src, int x, int y) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	return copy_from(src, rect);
}

bool Image::copy_from(const Image& src, SDL_Rect& dst_rect) {
	if(!image) {
		image = SDL_ConvertSurface(src.image, src.image->format,
					   src.image->flags);
		if(!image) {
			App::error_string.push_back(std::string("SDL_ConvertSurface"
				" failed: ") + SDL_GetError());
			return false;
		}
	}
	if(SDL_BlitSurface(src.image, nullptr, image, &dst_rect) < 0) {
		App::error_string.push_back(std::string("SDL_BlitSurface"
			" failed: ") + SDL_GetError());
		return false;
	}
	return true;
}

bool Image::copy_from(const Image& src, SDL_Rect& dst_rect,
					const SDL_Rect& src_rect) {
	if(!image) {
		image = SDL_ConvertSurface(src.image, src.image->format,
					   src.image->flags);
		if(!image) {
			App::error_string.push_back(std::string("SDL_ConvertSurface"
				" failed: ") + SDL_GetError());
			return false;
		}
	}
	if(SDL_BlitSurface(src.image, &src_rect, image, &dst_rect) < 0) {
		App::error_string.push_back(std::string("SDL_BlitSurface"
			" failed: ") + SDL_GetError());
		return false;
	}
	return true;
}

/*bool Image::copy_scaled() {
	SDL_BlitScaled(src,srcrect, dst, dstrect);
}*/

void Image::vertical_flip() {
	if(!image)
		return;

	unsigned int bpp = image->format->BytesPerPixel;

	std::unique_ptr<unsigned char[]> buf = std::make_unique<unsigned char[]>(width * height * bpp);

	for(unsigned int y = 0; y < height; y++) {
		for(unsigned int x = 0; x < width; x++) {
			for(unsigned int c = 0; c < bpp; c++) {
				buf[(y * width + x) * bpp + c] =
					((unsigned char *)image->pixels)[((height - y - 1) * width + x) * bpp + c];
			}
		}
	}

	memcpy(image->pixels, buf.get(), width * height * bpp);
}

void Image::horizontal_flip() {
	if(!image)
		return;

	unsigned int bpp = image->format->BytesPerPixel;

	std::unique_ptr<unsigned char[]> buf = std::make_unique<unsigned char[]>(width * height * bpp);

	for(unsigned int y = 0; y < height; y++) {
		for(unsigned int x = 0; x < width; x++) {
			for(unsigned int c = 0; c < bpp; c++) {
				buf[(y * width + x) * bpp + c] =
					((unsigned char *)image->pixels)[(y * width + (width - x - 1)) * bpp + c];
			}
		}
	}

	memcpy(image->pixels, buf.get(), width * height * bpp);
}

void Image::set_color_key(int r, int g, int b, bool enable) {
	SDL_SetColorKey(image, enable, SDL_MapRGB(image->format, r, g, b));
}

void Image::add_path(std::string path) {
	if(path[path.length() - 1] != '/')
		path += '/';

	if(std::find(paths.begin(), paths.end(), path) == paths.end())
		paths.push_back(path);
}
