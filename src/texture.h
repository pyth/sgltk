#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "app.h"
#include "image.h"

namespace sgltk {

/**
 * @class Texture
 * @brief Manages textures
 */
class Texture {
	/**
	 * @brief Contains stored textures
	 */
	EXPORT static std::map<std::string, Texture *> textures;
public:
	/**
	 * @brief The texture name
	 */
	GLuint texture;
	/**
	 * @brief The width of the texture
	 */
	unsigned int width;
	/**
	 * @brief The height of the texture
	 */
	unsigned int height;

	EXPORT Texture();
	EXPORT ~Texture();

	/**
	 * @brief Stores a texture object in an internal map using the
	 * 	  name parameter as key (no duplicates)
	 * @param name The key used to store the texture
	 * @param texture The texture to be stored
	 * @return Returns true if the insertion was successful, false otherwise
	 */
	EXPORT static bool store_texture(std::string name, Texture *texture);
	/**
	 * @brief Finds a texture in the internal map using the name as
	 * 	  key
	 * @param name The key used to find the texture
	 * @return Returns a pointer to the found texture object or NULL 
	 * 	   if no texture with such name could be found.
	 */
	EXPORT static Texture *find_texture(std::string name);

	/**
	* @brief Sets a texture parameter
	* @param name The name of the texture parameter
	* @param parameter The parameter value
	*/
	virtual void set_parameter(GLenum name, int parameter) = 0;
	/**
	* @brief Sets a texture parameter
	* @param name The name of the texture parameter
	* @param parameter The parameter value
	*/
	virtual void set_parameter(GLenum name, float parameter) = 0;
	/**
	 * @brief Bind the texture to be used by the shader
	 * @param texture_unit The texture unit to bind the texture to
	 */
	virtual void bind(unsigned int texture_unit = 0) = 0;
	/**
	 * @brief Unbind the texture
	 * @param texture_unit The texture unit to bind the texture to
	 */
	virtual void unbind(unsigned int texture_unit = 0) = 0;
};


class Texture_2d : public Texture {
public:
	EXPORT Texture_2d();
	/**
	 * @param path Path to the image file to load
	 */
	EXPORT Texture_2d(const std::string& path);
	/**
	* @param image The image to use as the texture
	*/
	EXPORT Texture_2d(const Image& image);
	/**
	 * @param res_x The texture x resolution
	 * @param res_y The texture y resolution
	 * @param internal_format The internal format of the texture
	 * @param type The type of pixel data
	 * @param format The format of the textre
	 */
	EXPORT Texture_2d(unsigned int res_x, unsigned int res_y, GLenum internal_format, GLenum type, GLenum format);
	EXPORT ~Texture_2d();

	/**
	* @brief Sets a texture parameter
	* @param name The name of the texture parameter
	* @param parameter The parameter value
	*/
	EXPORT void set_parameter(GLenum name, int parameter);
	/**
	* @brief Sets a texture parameter
	* @param name The name of the texture parameter
	* @param parameter The parameter value
	*/
	EXPORT void set_parameter(GLenum name, float parameter);
	/**
	* @brief Creates an empty texture
	* @param res_x The texture x resolution
	* @param res_y The texture y resolution
	* @param internal_format The internal format of the texture
	* @param type The type of pixel data
	* @param format The format of the textre
	*/
	EXPORT void create_empty(unsigned int res_x,
		unsigned int res_y,
		GLenum internal_format,
		GLenum type,
		GLenum format);
	/**
	 * @brief Load a new image as texture
	 * @param image The image to use as the texture
	 */
	EXPORT bool load(const Image& image);
	/**
	 * @brief Load a new image as texture
	 * @param path The path to the image file
	 */
	EXPORT bool load(const std::string& path);
	/**
	* @brief Bind the texture to be used by the shader
	* @param texture_unit The texture unit to bind the texture to
	*/
	EXPORT void bind(unsigned int texture_unit = 0);
	/**
	* @brief Unbind the texture
	* @param texture_unit The texture unit to bind the texture to
	*/
	EXPORT void unbind(unsigned int texture_unit = 0);
};


class Cubemap : public Texture {
public:
	EXPORT Cubemap();
	/**
	 * @param res_x The texture x resolution
	 * @param res_y The texture y resolution
	 * @param internal_format The internal format of the texture
	 * @param type The type of pixel data
	 * @param format The format of the textre
	 */
	EXPORT Cubemap(unsigned int res_x,
		       unsigned int res_y,
		       GLenum internal_format,
		       GLenum type,
		       GLenum format);
	/**
	 * @brief Load a new cubemap
	 * @param pos_x The image to use as the positive x texture
	 * @param neg_x The image to use as the negative x texture
	 * @param pos_y The image to use as the positive y texture
	 * @param neg_y The image to use as the negative y texture
	 * @param pos_z The image to use as the positive z texture
	 * @param neg_z The image to use as the negative z texture
	 */
	EXPORT Cubemap(const Image& pos_x, const Image& neg_x,
				const Image& pos_y, const Image& neg_y,
				const Image& pos_z, const Image& neg_z);
	EXPORT ~Cubemap();

	/**
	* @brief Sets a texture parameter
	* @param name The name of the texture parameter
	* @param parameter The parameter value
	*/
	EXPORT void set_parameter(GLenum name, int parameter);
	/**
	* @brief Sets a texture parameter
	* @param name The name of the texture parameter
	* @param parameter The parameter value
	*/
	EXPORT void set_parameter(GLenum name, float parameter);
	/**
	 * @brief Creates an empty texture
	 * @param res_x The texture x resolution
	 * @param res_y The texture y resolution
	 * @param internal_format The internal format of the texture
	 * @param type The type of pixel data
	 * @param format The format of the textre
	 */
	EXPORT void create_empty(unsigned int res_x,
				 unsigned int res_y,
				 GLenum internal_format,
				 GLenum type,
				 GLenum format);
	/**
	 * @brief Load a new cubemap
	 * @param pos_x The image to use as the positive x texture
	 * @param neg_x The image to use as the negative x texture
	 * @param pos_y The image to use as the positive y texture
	 * @param neg_y The image to use as the negative y texture
	 * @param pos_z The image to use as the positive z texture
	 * @param neg_z The image to use as the negative z texture
	 */
	EXPORT bool load(const Image& pos_x, const Image& neg_x,
			 const Image& pos_y, const Image& neg_y,
			 const Image& pos_z, const Image& neg_z);
	/**
	 * @brief Bind the texture to be used by the shader
	 * @param texture_unit The texture unit to bind the texture to
	 */
	EXPORT void bind(unsigned int texture_unit = 0);
	/**
	 * @brief Unbind the texture
	 * @param texture_unit The texture unit to bind the texture to
	 */
	EXPORT void unbind(unsigned int texture_unit = 0);
};
}

#endif
