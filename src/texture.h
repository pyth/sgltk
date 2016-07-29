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

	/**
	 * @param target The target to bind the texture
	 */
	EXPORT Texture(GLenum target = GL_TEXTURE_2D);
	/**
	 * @param path The path to the image file
	 */
	EXPORT Texture(std::string path);
	/**
	 * @param target The target to bind the texture
	 * @param path The path to the image file
	 */
	EXPORT Texture(GLenum target, std::string path);
	/**
	 * @param image The image to use as the texture
	 */
	EXPORT Texture(Image *image);
	/**
	 * @param target The target to bind the texture
	 * @param image The image to use as the texture
	 */
	EXPORT Texture(GLenum target, Image *image);
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
	 * @brief Load a new image
	 * @param image The image to use as the texture
	 */
	EXPORT void load_texture(Image *image);
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
