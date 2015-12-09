#ifndef __SHADER_H__
#define __SHADER_H__

#include "core.h"

namespace sgltk {
	enum UNIFORM_TYPE {
		INTEGER, /**A single integer*/
		VECTOR2I, /**A 2d integer vector*/
		VECTOR3I, /**A 3d integer vector*/
		VECTOR4I, /**A 4d integer vector*/

		FLOAT, /**A single float*/
		VECTOR2F, /**A 2d float vector*/
		VECTOR3F, /**A 3d float vector*/
		VECTOR4F, /**A 4d float vector*/

		MATRIX3, /**A 3x3 float matrix*/
		MATRIX4 /**A 4x4 float matrix*/
	};
}

/**
 * @class Shader
 * @brief Manager shaders
 */
class Shader {
	bool modify;

	std::map<const char *, GLenum> shader_path_map;

	std::map<const char *, GLenum> shader_string_map;
public:
	/**
	 * @brief Shader name
	 */
	GLuint shader;

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
	 * @brief Binds a uniform to the shader
	 * @param name		Uniform name
	 * @param type		Uniform type
	 * @param data		Pointer to the data
	 * Warning! Function not yet implemented
	 */
	void bind_uniform(char *name, sgltk::UNIFORM_TYPE type, void *data);
	/**
	 * @brief Links the attached shaders
	 */
	void link();
	/**
	 * @brief Binds the shader
	 */
	void bind();
	/**
	 * @brief Unbinds the shader
	 */
	void unbind();
};

#endif
