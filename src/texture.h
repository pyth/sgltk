#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "core.h"

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

/**
 * @class Texture
 * @brief Manages textures
 */
class Texture {
	GLenum target;
	/**
	 * @brief Contains stored textures
	 */
	static std::map<std::string, Texture *> textures;
public:
	/**
	 * @brief The texture name
	 */
	GLuint texture;

	Texture();
	/**
	 * @param target The target to bind the texture
	 */
	Texture(GLenum target);
	/**
	 * @param path The path to the image file
	 */
	Texture(std::string path);
	/**
	 * @param target The target to bind the texture
	 * @param path The path to the image file
	 */
	Texture(GLenum target, std::string path);
	/**
	 * @param image The image to use as the texture
	 */
	Texture(Image *image);
	/**
	 * @param target The target to bind the texture
	 * @param image The image to use as the texture
	 */
	Texture(GLenum target, Image *image);
	~Texture();

	/**
	 * @brief Stores a texture object in an internal map using the
	 * 	  name parameter as key (no duplicates)
	 * @param name The key used to store the texture
	 * @param texture The texture to be stored
	 * @return Returns true if the insertion was successful, false otherwise
	 */
	static bool store_texture(std::string name, Texture *texture);
	/**
	 * @brief Finds a texture in the internal map using the name as
	 * 	  key
	 * @param name The key used to find the texture
	 * @return Returns a pointer to the found texture object or NULL 
	 * 	   if no texture with such name could be found.
	 */
	static Texture *find_texture(std::string name);
	/**
	 * @brief Load a new image
	 * @param image The image to use as the texture
	 */
	void load_texture(Image *image);
	/**
	 * @brief Sets a texture parameter
	 * @param name The name of the texture parameter
	 * @param parameter The parameter value
	 */
	void set_parameter(GLenum name, int parameter);
	/**
	 * @brief Sets a texture parameter
	 * @param name The name of the texture parameter
	 * @param parameter The parameter value
	 */
	void set_parameter(GLenum name, float parameter);
	/**
	 * @brief Bind the texture to be used by the shader
	 */
	void bind();
	/**
	 * @brief Bind the texture to be used by the shader
	 * @param texture_unit The texture unit to bind the texture to
	 */
	void bind(unsigned int texture_unit);
	/**
	 * @brief Unbind the texture
	 */
	void unbind();
	/**
	 * @brief Unbind the texture
	 * @param texture_unit The texture unit to bind the texture to
	 */
	void unbind(unsigned int texture_unit);
};

#endif
