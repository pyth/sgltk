#ifndef __SHADER_H__
#define __SHADER_H__

#include "app.h"

namespace sgltk {

/**
 * @class Shader
 * @brief Manager shaders
 */
class Shader {
	bool modify;

	GLuint saved_program;

	std::map<std::string, GLenum> shader_path_map;

	std::map<std::string, GLenum> shader_string_map;

	static std::vector<std::string> paths;
public:
	/**
	 * @brief Adds a path to the list of paths to be searched
	 *▸       for image files.
	 * @param path The path to add to the list
	 * @note To avoid duplicates this function first performs
	 *▸      a search an existing entry path.
	 */
	EXPORT static void add_path(std::string path);

	/**
	 * @brief Shader name
	 */
	GLuint program;

	EXPORT Shader();
	EXPORT ~Shader();

	/**
	 * @brief Compiles and attaches shader files
	 * @param filename	Path to a shader source file
	 * @param type		Shader type
	 * @return		Returns true on success or false otherwise
	 */
	EXPORT bool attach_file(const std::string& filename, GLenum type);
	/**
	 * @brief Compiles and attaches shader strings
	 * @param shader_string	A string containing the shader
	 * @param type		Shader type
	 * @return		Returns true on success or false otherwise
	 */
	EXPORT bool attach_string(const std::string& shader_string, GLenum type);
	/**
	 * @brief Reads, compiles and links all associated shaders again.
	 */
	EXPORT void recompile();
	/**
	 * @brief Links the attached shaders
	 */
	EXPORT void link();
	/**
	 * @brief Binds the shader and stores the previously bound shader
	 */
	EXPORT void bind();
	/**
	 * @brief Unbinds the shader and restores the previously bound shader
	 */
	EXPORT void unbind();
};
}

#endif
