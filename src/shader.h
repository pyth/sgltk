#ifndef __SHADER_H__
#define __SHADER_H__

#include "core.h"

enum UniformType {
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
	 * @brief Reads, compiles and links all associated shaders again.
	 */
	void recompile();
	/**
	 * @brief Binds a uniform to the shader
	 * @param name		Uniform name
	 * @param type		Uniform type
	 * @param data		Pointer to the data
	 * Warning! Function not implemented yet
	 */
	void bind_uniform(char *name, UniformType type, void *data);
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
