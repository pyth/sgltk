#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "app.h"

namespace sgltk {

/**
 * @class Image
 * @brief Manages images
 */
class Image {
	bool free_data;
	static std::vector<std::string> paths;
public:
	/**
	 * @brief Adds a path to the list of paths to be searched
	 *	  for image files.
	 * @param path The path to add to the list
	 * @note To avoid duplicates this function first performs
	 *	 a search an existing entry path.
	 */
	EXPORT static void add_path(std::string path);
	/**
	 * @brief The width of the image surface
	 */
	unsigned int width;
	/**
	 * @brief The width of the image surface
	 */
	unsigned int height;
	/**
	 * @brief The number of bytes that make up a single pixel
	 */
	unsigned int bytes_per_pixel;
	/**
	 * @brief The pixel data
	 */
	void *data;

	/**
	 * @brief The image surface
	 */
	SDL_Surface *image;

	EXPORT Image();
	/**
	 * @param filename Path to the image file to load
	 */
	EXPORT Image(std::string filename);
	/**
	 * @param width The width of the image
	 * @param height The height of the image
	 * @param bytes_per_pixel The number of bytes that represent a pixel
	 * @param data A pointer to the buffer data
	 */
	EXPORT Image(unsigned int width,
		     unsigned int height,
		     unsigned int bytes_per_pixel,
		     void *data);
	EXPORT ~Image();

	/**
	 * @brief Creates an empty image
	 * @param width The width of the image
	 * @param height The height of the image
	 * @return Returns true on success, false otherwise
	 */
	EXPORT bool create_empty(unsigned int width,
				 unsigned int height);

	/**
	 * @brief Loads a new image file
	 * @param filename Path to the image file to load
	 * @return Returns true on success, false otherwise
	 */
	EXPORT bool load(const std::string& filename);

	/**
	 * @brief Loads image from a data buffer
	 * @param width The width of the image
	 * @param height The height of the image
	 * @param bytes_per_pixel The number of bytes that represent a pixel
	 * @param data A pointer to the buffer data
	 * @return Returns true on success, false otherwise
	 * @note The image object keeps it's own copy of the data until a new image is loaded
	 */
	EXPORT bool load(unsigned int width,
			 unsigned int height,
			 unsigned int bytes_per_pixel,
			 void *data);

	/**
	 * @brief Saves the image as a file
	 * @param filename The path to save the image to
	 * @return Returns true on success, false otherwise
	 */
	EXPORT bool save(const std::string& filename);

#ifdef HAVE_SDL_TTF_H
	/**
	 * @brief Opens a TTF font file
	 * @param font_file The path to the TTF font file
	 * @param size The font size
	 * @return Return a ttf font file pointer on success, NULL on failure
	 * @see close_font_file()
	 */
	EXPORT static TTF_Font *open_font_file(const std::string& font_file,
					       unsigned int size);
	/**
	 * @brief Closes a ttf font file
	 * @param A pointer to the ttf font file to close
	 * @see open_font_file()
	 */
	EXPORT static void close_font_file(TTF_Font *font_file);
	/**
	 * @brief Creates an image from a string
	 * @param text		The string to be made into an image
	 * @param font		The opened font file
	 * @param r		The red color channel
	 * @param g		The green color channel
	 * @param b		The blue color channel
	 * @param a		The alpha color channel
	 * @return Retuurns true on success, false on failure
	 * @note If you already have a ttf file open this function is better
	 * 	for performance
	 * @see open_font_file()
	 * @see close_font_file()
	 */
	EXPORT bool create_text(const std::string& text,
				TTF_Font *font,
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
	 * @return Retuurns true on success, false on failure
	 */
	EXPORT bool create_text(const std::string& text,
				std::string font_file,
				unsigned int size,
				Uint8 r, Uint8 g, Uint8 b, Uint8 a);
#endif //HAVE_SDL_TTF_H

	/**
	 * @brief Copies an image into the current image
	 * @param src		The image to be copied
	 * @param x		The x coordinate to copy the image to
	 * @param y		The y coordinate to copy the image to
	 */
	EXPORT bool copy_from(const Image& src, int x, int y);
	/**
	 * @brief Copies an image into the current image
	 * @param src		The image to be copied
	 * @param dst_rect	The coordinates to copy the image to
	 */
	EXPORT bool copy_from(const Image& src, SDL_Rect& dst_rect);
	/**
	 * @brief Copies an image into the current image
	 * @param src		The image to be copied
	 * @param dst_rect	The coordinates to copy the image to
	 * @param src_rect	The part of the image to copy
	 */
	EXPORT bool copy_from(const Image& src, SDL_Rect& dst_rect,
						const SDL_Rect& src_rect);

	/**
	 * @brief Flips the image along its x-axis
	 */
	EXPORT void vertical_flip();

	/**
	 * @brief Flips the image along its y-axis
	 */
	EXPORT void horizontal_flip();

	/**
	 * @brief Sets a color to be transparent
	 * @param r		The red color channel
	 * @param g		The green color channel
	 * @param b		The blue color channel
	 */
	EXPORT void set_color_key(int r, int g, int b);
};

}

#endif
