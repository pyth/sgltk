#ifndef __SHADER_H__
#define __SHADER_H__

#include "core.h"

namespace sgltk {

/**
 * @class Shader
 * @brief Manager shaders
 */
class Shader {
	bool modify;

	GLuint saved_program;

	std::map<const char *, GLenum> shader_path_map;

	std::map<const char *, GLenum> shader_string_map;
public:
	/**
	 * @brief Shader name
	 */
	GLuint program;

	Shader();
	~Shader();

	/**
	 * @brief Compiles and attaches shader files
	 * @param filename	Path to a shader source file
	 * @param type		Shader type
	 * @return		Returns true on success or false otherwise
	 */
	bool attach_file(const char *filename, GLenum type);
	/**
	 * @brief Compiles and attaches shader strings
	 * @param shader_string	A string containing the shader
	 * @param size		The length of the string including the null character
	 * @param type		Shader type
	 * @return		Returns true on success or false otherwise
	 */
	bool attach_string(const char *shader_string, GLint size, GLenum type);
	/**
	 * @brief Reads, compiles and links all associated shaders again.
	 */
	void recompile();
	/**
	 * @brief Links the attached shaders
	 */
	void link();
	/**
	 * @brief Binds the shader and stores the previously bound shader
	 */
	void bind();
	/**
	 * @brief Unbinds the shader and restores the previously bound shader
	 */
	void unbind();
};
}

#endif
