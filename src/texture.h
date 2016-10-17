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
	static std::map<std::string, Texture *> textures;
public:
	/**
	 * @brief The texture target
	 */
	GLenum target;
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
	/**
	 * @param target The target to bind the texture
	 * @param res_x The texture x resolution
	 * @param res_y The texture y resolution
	 * @param internal_format The internal format of the texture
	 * @param type The type of pixel data
	 * @param format The format of the textre
	 */
	EXPORT Texture(GLenum target,
		       unsigned int res_x,
		       unsigned int res_y,
		       GLenum internal_format,
		       GLenum type,
		       GLenum format);
	/**
	 * @param target The target to bind the texture
	 */
	EXPORT Texture(GLenum target = GL_TEXTURE_2D);
	/**
	 * @param path The path to the image file
	 */
	EXPORT Texture(const std::string& path);
	/**
	 * @param target The target to bind the texture
	 * @param path The path to the image file
	 */
	EXPORT Texture(GLenum target, const std::string& path);
	/**
	 * @param image The image to use as the texture
	 */
	EXPORT Texture(const Image& image);
	/**
	 * @param target The target to bind the texture
	 * @param image The image to use as the texture
	 */
	EXPORT Texture(GLenum target, const Image& image);
	EXPORT ~Texture();

	/**
	 * @brief Sets the target to which the texture is bound
	 * @param The target to which the texture is bound
	 */
	EXPORT void set_target(GLenum target);
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
	EXPORT void load_texture(const Image& image);
	/**
	 * @param target The target to bind the texture
	 * @param path The path to the image file
	 */
	EXPORT void load_texture(const std::string& path);
	/**
	 * @brief Load a new cubemap
	 * @param pos_x The image to use as the positive x texture
	 * @param neg_x The image to use as the negative x texture
	 * @param pos_y The image to use as the positive y texture
	 * @param neg_y The image to use as the negative y texture
	 * @param pos_z The image to use as the positive z texture
	 * @param neg_z The image to use as the negative z texture
	 */
	EXPORT void load_cubemap(const Image& pos_x, const Image& neg_x,
				 const Image& pos_y, const Image& neg_y,
				 const Image& pos_z, const Image& neg_z);
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
	 * @brief Bind the texture to be used by the shader
	 */
	EXPORT void bind();
	/**
	 * @brief Bind the texture to be used by the shader
	 * @param texture_unit The texture unit to bind the texture to
	 */
	EXPORT void bind(unsigned int texture_unit);
	/**
	 * @brief Unbind the texture
	 */
	EXPORT void unbind();
	/**
	 * @brief Unbind the texture
	 * @param texture_unit The texture unit to bind the texture to
	 */
	EXPORT void unbind(unsigned int texture_unit);
};
}

#endif
