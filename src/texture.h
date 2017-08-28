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
	 * @brief Adds a path to the list of paths to be searched
	 *	  for image files.
	 * @param path The path to add to the list
	 * @note To avoid duplicates this function first performs
	 *	 a search on existing entries.
	 */
	EXPORT static void add_path(std::string path);
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
	 * @brief The number of layers
	 */
	unsigned int num_layers;

	EXPORT Texture();
	EXPORT ~Texture();

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
	* @brief Sets a texture parameter
	* @param name The name of the texture parameter
	* @param parameter The parameter value
	*/
	EXPORT void set_parameter(GLenum name, float *parameter);

	/**
	 * @brief Stores a texture object in an internal map using the
	 * 	  name parameter as key (no duplicates)
	 * @param name The key used to store the texture
	 * @param texture The texture to be stored
	 * @return Returns true if the insertion was successful, false otherwise
	 */
	EXPORT static bool store_texture(std::string name, Texture *texture);
	/**
	 * @brief Finds a texture in the internal map using the name as key
	 * @param name The key used to find the texture
	 * @return Returns a pointer to the found texture object or NULL
	 * 	   if no texture with such name could be found.
	 */
	EXPORT static Texture *find_texture(std::string name);
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
	/**
	 * @brief Binds a level of a texture to an image unit
	 * @param unit The index of the image unit to which to bind the texture
	 * @param level The level of the texture that is to be bound
	 * @param layered Specifies whether a layered texture binding
	 * 	is to be established
	 * @param layer If layered is GL_FALSE, specifies the layer of texture
	 * 	to be bound to the image unit, ignored otherwise
	 * @param access Specifies the type of access that will
	 * 	be performed on the image
	 * @param format Specifies the format that the elements of the image
	 * 	will be treated as
	 */
	EXPORT void bind_image(GLuint unit, GLint level,
			       GLboolean layered, GLint layer,
			       GLenum access, GLenum format);
};


/**
 * @class Texture_1d
 * @brief Manages 1d textures
 */
class Texture_1d : public Texture {
public:
	EXPORT Texture_1d();
	/**
	 * @param path Path to the image file to load
	 */
	EXPORT Texture_1d(const std::string& path);
	/**
	* @param image The image to use as the texture
	*/
	EXPORT Texture_1d(const sgltk::Image& image);
	/**
	 * @param res The texture resolution
	 * @param internal_format The internal format of the texture
	 * @param type The type of pixel data
	 * @param format The format of the textre
	 */
	EXPORT Texture_1d(unsigned int res,
			  GLenum internal_format,
			  GLenum type, GLenum format);
	EXPORT ~Texture_1d();

	/**
	* @brief Creates an empty texture
	* @param res The texture resolution
	* @param internal_format The internal format of the texture
	* @param type The type of pixel data
	* @param format The format of the textre
	*/
	EXPORT void create_empty(unsigned int res,
				 GLenum internal_format,
				 GLenum type,
				 GLenum format);
	/**
	 * @brief Load a new image as texture
	 * @param image The image to use as the texture
	 */
	EXPORT bool load(const sgltk::Image& image);
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


/**
 * @class Texture_1d_Array
 * @brief Manages 1d texture arrays
 */
class Texture_1d_Array : public Texture {
public:
	EXPORT Texture_1d_Array();
	/**
	 * @param paths Paths to the image files to load
	 */
	EXPORT Texture_1d_Array(const std::vector<std::string>& paths);
	/**
	* @param images The images to use as the texture layers
	*/
	EXPORT Texture_1d_Array(const std::vector<sgltk::Image>& images);
	/**
	 * @param res The texture resolution
	 * @param num_layers The number of layers
	 * @param internal_format The internal format of the texture
	 * @param type The type of pixel data
	 * @param format The format of the textre
	 *
	 */
	EXPORT Texture_1d_Array(unsigned int res,
				unsigned int num_layers,
				GLenum internal_format,
				GLenum type, GLenum format);
	EXPORT ~Texture_1d_Array();

	/**
	* @brief Creates an empty texture
	* @param res The texture resolution
	* @param num_layers The number of layers
	* @param internal_format The internal format of the texture
	* @param type The type of pixel data
	* @param format The format of the textre
	*/
	EXPORT void create_empty(unsigned int res,
				 unsigned int num_layers,
				 GLenum internal_format,
				 GLenum type,
				 GLenum format);
	/**
	 * @brief Load a new image as texture
	 * @param images The images to use as the texture layers
	 */
	EXPORT bool load(const std::vector<sgltk::Image>& images);
	/**
	 * @brief Load a new image as texture
	 * @param paths The path to the image file
	 */
	EXPORT bool load(const std::vector<std::string>& paths);
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


/**
 * @class Texture_2d
 * @brief Manages 2d textures
 */
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
	EXPORT Texture_2d(const sgltk::Image& image);
	/**
	 * @param res_x The texture x resolution
	 * @param res_y The texture y resolution
	 * @param internal_format The internal format of the texture
	 * @param type The type of pixel data
	 * @param format The format of the textre
	 */
	EXPORT Texture_2d(unsigned int res_x,
			  unsigned int res_y,
			  GLenum internal_format,
			  GLenum type, GLenum format);
	EXPORT ~Texture_2d();

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
	EXPORT bool load(const sgltk::Image& image);
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

/**
 * @class Texture_2d_Array
 * @brief Manages 2d texture arrays
 */
class Texture_2d_Array : public Texture {
public:
	EXPORT Texture_2d_Array();
	/**
	 * @param paths Paths to the image files to load
	 */
	EXPORT Texture_2d_Array(const std::vector<std::string>& paths);
	/**
	* @param images The images to use as the texture layers
	*/
	EXPORT Texture_2d_Array(const std::vector<sgltk::Image>& images);
	/**
	 * @param res_x The texture x resolution
	 * @param res_y The texture y resolution
	 * @param num_layers The number of layers
	 * @param internal_format The internal format of the texture
	 * @param type The type of pixel data
	 * @param format The format of the textre
	 *
	 */
	EXPORT Texture_2d_Array(unsigned int res_x,
				unsigned int res_y,
				unsigned int num_layers,
				GLenum internal_format,
				GLenum type, GLenum format);
	EXPORT ~Texture_2d_Array();

	/**
	* @brief Creates an empty texture
	* @param res_x The texture x resolution
	* @param res_y The texture y resolution
	* @param num_layers The number of layers
	* @param internal_format The internal format of the texture
	* @param type The type of pixel data
	* @param format The format of the textre
	*/
	EXPORT void create_empty(unsigned int res_x,
				 unsigned int res_y,
				 unsigned int num_layers,
				 GLenum internal_format,
				 GLenum type,
				 GLenum format);
	/**
	 * @brief Load a new image as texture
	 * @param images The images to use as the texture layers
	 */
	EXPORT bool load(const std::vector<sgltk::Image>& images);
	/**
	 * @brief Load a new image as texture
	 * @param paths The path to the image file
	 */
	EXPORT bool load(const std::vector<std::string>& paths);
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

/**
* @class Texture_3d
* @brief Manages 3d textures
*/
class Texture_3d : public Texture {
public:
	EXPORT Texture_3d();
	/**
	* @param paths Path to the image file to load
	*/
	EXPORT Texture_3d(const std::vector<std::string>& paths);
	/**
	* @param images The image to use as the texture
	*/
	EXPORT Texture_3d(const std::vector<sgltk::Image>& images);
	/**
	* @param res_x The texture x resolution
	* @param res_y The texture y resolution
	* @param res_z The texture z resolution
	* @param internal_format The internal format of the texture
	* @param type The type of pixel data
	* @param format The format of the textre
	*/
	EXPORT Texture_3d(unsigned int res_x,
			  unsigned int res_y,
			  unsigned int res_z,
			  GLenum internal_format,
			  GLenum type,
			  GLenum format);
	EXPORT ~Texture_3d();

	/**
	* @brief Creates an empty texture
	* @param res_x The texture x resolution
	* @param res_y The texture y resolution
	* @param res_z The texture z resolution
	* @param internal_format The internal format of the texture
	* @param type The type of pixel data
	* @param format The format of the textre
	*/
	EXPORT void create_empty(unsigned int res_x,
				 unsigned int res_y,
				 unsigned int res_z,
				 GLenum internal_format,
				 GLenum type,
				 GLenum format);
	/**
	* @brief Load a new image as texture
	* @param images The image to use as the texture
	*/
	EXPORT bool load(const std::vector<sgltk::Image>& images);
	/**
	* @brief Load a new image as texture
	* @param paths The path to the image file
	*/
	EXPORT bool load(const std::vector<std::string>& paths);
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

/**
 * @class Cubemap
 * @brief Manages cubemaps
 */
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
	EXPORT Cubemap(const sgltk::Image& pos_x, const sgltk::Image& neg_x,
		       const sgltk::Image& pos_y, const sgltk::Image& neg_y,
		       const sgltk::Image& pos_z, const sgltk::Image& neg_z);
	EXPORT ~Cubemap();

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
	EXPORT bool load(const std::string& pos_x, const std::string& neg_x,
			 const std::string& pos_y, const std::string& neg_y,
			 const std::string& pos_z, const std::string& neg_z);
	/**
	 * @brief Load a new cubemap
	 * @param pos_x The image to use as the positive x texture
	 * @param neg_x The image to use as the negative x texture
	 * @param pos_y The image to use as the positive y texture
	 * @param neg_y The image to use as the negative y texture
	 * @param pos_z The image to use as the positive z texture
	 * @param neg_z The image to use as the negative z texture
	 */
	EXPORT bool load(const sgltk::Image& pos_x, const sgltk::Image& neg_x,
			 const sgltk::Image& pos_y, const sgltk::Image& neg_y,
			 const sgltk::Image& pos_z, const sgltk::Image& neg_z);
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
