#ifndef __SHADER_H__
#define __SHADER_H__

#include "core.h"

enum UniformType {
	INTEGER1,
	FLOAT1,
	VECTOR3F,
	VECTOR4F,
	MATRIX3,
	MATRIX4
};

/**
 * @class Shader
 * @brief Manager shaders
 */
class Shader {
	bool modify;
	std::map<const char *, GLenum> shader_map;
	std::vector<const char *> shader_paths;
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
	bool attach(const char *filename, GLenum type);
	/**
	 * @brief Reads, compiles, attaches and links all associated shaders again.
	 */
	void recompile();
	/**
	 * @brief Binds a uniform to the shader
	 * @param type		Uniform type
	 * @param name		Uniform name
	 * @param data		Pointer to the data
	 * Warning! Function not implemented yet
	 */
	void bind_uniform(UniformType type, char *name, void *data);
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
