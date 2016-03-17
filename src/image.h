#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "core.h"

namespace sgltk {

/**
 * @class Image
 * @brief Manages images
 */
class Image {
	static std::vector<std::string> paths;
public:
	/**
	 * @brief Adds a path to the list of paths to be searched
	 *	  for image files.
	 * @param path The path to add to the list
	 * @note To avoid duplacates this function first performs
	 *	 a search an existing entry path.
	 */
	static void add_path(std::string path);
	/**
	 * @brief The width of the image surface
	 */
	int width;
	/**
	 * @brief The width of the image surface
	 */
	int height;

	/**
	 * @brief The image surface
	 */
	SDL_Surface *image;

	Image();
	/**
	 * @param filename Path to the image file to load
	 */
	Image(std::string filename);
	~Image();

	/**
	 * @brief Creates an empty image
	 * @param width The width of the image
	 * @param height The height of the image
	 */
	bool create_empty(int width, int height);

	/**
	 * @brief Loads a new image file
	 * @param filename Path to the image file to load
	 */
	bool load(std::string filename);

	/**
	 * @brief Creates an image from a string
	 * @param text		The string to be made into an image
	 * @param font		The opened font file
	 * @param size		The font size
	 * @param r		The red color channel
	 * @param g		The green color channel
	 * @param b		The blue color channel
	 * @param a		The alpha color channel
	 * If you already have a ttf file open this function is better
	 * 	for performance
	 */
	bool create_text(std::string text, TTF_Font *font, int size,
			 Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	/**
	 * @brief Creates an image from a string
	 * @param text		The string to be made into an image
	 * @param font_file	The opened font file
	 * @param size		The font size
	 * @param r		The red color channel
	 * @param g		The green color channel
	 * @param b		The blue color channel
	 * @param a		The alpha color channel
	 */
	bool create_text(std::string text, std::string font_file, int size,
			 Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	/**
	 * @brief Copies an image into the current image
	 * @param src		The image to be copied
	 * @param x		The x coordinate to copy the image to
	 * @param y		The y coordinate to copy the image to
	 */
	bool copy_from(const Image *src, int x, int y);
	/**
	 * @brief Copies an image into the current image
	 * @param src		The image to be copied
	 * @param dst_rect	The coordinates to copy the image to
	 */
	bool copy_from(const Image *src, SDL_Rect *dst_rect);
	/**
	 * @brief Copies an image into the current image
	 * @param src		The image to be copied
	 * @param dst_rect	The coordinates to copy the image to
	 * @param src_rect	The part of the image to copy
	 */
	bool copy_from(const Image *src, SDL_Rect *dst_rect,
		       SDL_Rect *src_rect);

	/**
	 * @brief Sets a color to be transparent
	 * @param r		The red color channel
	 * @param g		The green color channel
	 * @param b		The blue color channel
	 */
	void set_color_key(int r, int g, int b);
};

}

#endif