#ifndef __SHADER_H__
#define __SHADER_H__

#include "app.h"

namespace sgltk {

/**
 * @class Shader
 * @brief Manager shaders
 */
class Shader {
	EXPORT static int counter;

	//The id of the currently bound shader object
	EXPORT static int bound;

	//The id of the shader object
	int id;

	bool modify;
	bool linked;

	std::vector<GLuint> attached;

	std::map<std::string, GLenum> shader_path_map;

	std::map<std::string, GLenum> shader_string_map;

	static std::vector<std::string> paths;
public:
	/**
	 * @brief True if the shader records values into transform feedback
	 * 	buffer(s)
	 */
	bool transform_feedback;
	/**
	 * @brief Adds a path to the list of paths to be searched
	 *▸       for image files.
	 * @param path The path to add to the list
	 * @note To avoid duplicates this function first performs
	 * 	 a search on existing entries.
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
	 * @brief Sets the variable to record in transform feedback buffers
	 * @param variables A list of variable names
	 * @param buffer_mode The mode used to capture the variables when
	 * 	transform feedback is active. Must be must be
	 * 	GL_INTERLEAVED_ATTRIBS or GL_SEPARATE_ATTRIBS.
	 * @note If the shader has already been linked, it will be relinked.
	 * @see link
	 */
	EXPORT void set_transform_feedback_variables(std::vector<std::string>& variables, GLenum buffer_mode);
	/**
	 * @brief Reads, compiles and links all associated shaders again.
	 */
	EXPORT void recompile();
	/**
	 * @brief Links the attached shaders
	 * @return Returns true on success, false otherwise
	 */
	EXPORT bool link();
	/**
	 * @brief Binds the shader and stores the previously bound shader
	 */
	EXPORT void bind();
	/**
	 * @brief Unbinds the shader and restores the previously bound shader
	 */
	EXPORT void unbind();
	/**
	 * @brief Returns the location of an attribute variable
	 * @param name The name of the attribute variable
	 * @return Returns the location of the attribute variable or -1 if it is not found
	 */
	EXPORT int get_attribute_location(const std::string& name);
	/**
	 * @brief Returns the location of a uniform variable
	 * @param name The name of the uniform variable
	 * @return Returns the location of the uniform variable or -1 if it is not found
	 */
	EXPORT int get_uniform_location(const std::string& name);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value
	 */
	template <typename T>
	void set_uniform(int location, T v0);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value
	 */
	template <typename T>
	void set_uniform(const std::string& name, T v0);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 */
	template <typename T>
	void set_uniform(int location, T v0, T v1);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 */
	template <typename T>
	void set_uniform(const std::string& name, T v0, T v1);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 */
	template <typename T>
	void set_uniform(int location, T v0, T v1, T v2);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 */
	template <typename T>
	void set_uniform(const std::string& name, T v0, T v1, T v2);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 * @param v3 The value of the fourth element
	 */
	template <typename T>
	void set_uniform(int location, T v0, T v1, T v2, T v3);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 * @param v3 The value of the fourth element
	 */
	template <typename T>
	void set_uniform(const std::string& name, T v0, T v1, T v2, T v3);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param count The number of vectors
	 * @param elements The number of elements per vector
	 * @param value A pointer to the values
	 */
	template <typename T>
	void set_uniform(int location, unsigned int count,
			 unsigned int elements, const T *value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param count The number of vectors
	 * @param elements The number of elements per vector
	 * @param value A pointer to the values
	 */
	template <typename T>
	void set_uniform(const std::string& name, unsigned int count,
			 unsigned int elements, const T *value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param count The number of vectors
	 * @param columns The number of matrix columns
	 * @param rows The number of matrix rows
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A pointer to the values
	 */
	template <typename T>
	void set_uniform(int location,
			 unsigned int count,
			 unsigned int columns,
			 unsigned int rows,
			 bool transpose,
			 const T *value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param count The number of vectors
	 * @param columns The number of matrix columns
	 * @param rows The number of matrix rows
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A pointer to the values
	 */
	template <typename T>
	void set_uniform(const std::string& name,
			 unsigned int count,
			 unsigned int columns,
			 unsigned int rows,
			 bool transpose,
			 const T *value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location, const glm::vec2& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location, const glm::dvec2& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location, const glm::vec3& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location, const glm::dvec3& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location, const glm::vec4& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location, const glm::dvec4& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const glm::vec2& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const glm::dvec2& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const glm::vec3& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const glm::dvec3& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const glm::vec4& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const glm::dvec4& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const glm::mat2& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const glm::dmat2& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const glm::mat3& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const glm::dmat3& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const glm::mat4& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const glm::dmat4& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const glm::mat2& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const glm::dmat2& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const glm::mat3& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const glm::dmat3& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const glm::mat4& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const glm::dmat4& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				const std::vector<glm::vec2>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				const std::vector<glm::dvec2>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				const std::vector<glm::vec3>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				const std::vector<glm::dvec3>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				const std::vector<glm::vec4>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				const std::vector<glm::dvec4>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const std::vector<glm::vec2>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const std::vector<glm::dvec2>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const std::vector<glm::vec3>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const std::vector<glm::dvec3>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const std::vector<glm::vec4>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const std::vector<glm::dvec4>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const std::vector<glm::mat2>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const std::vector<glm::dmat2>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const std::vector<glm::mat3>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const std::vector<glm::dmat3>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const std::vector<glm::mat4>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				bool transpose,
				const std::vector<glm::dmat4>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const std::vector<glm::mat2>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const std::vector<glm::dmat2>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const std::vector<glm::mat3>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const std::vector<glm::dmat3>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const std::vector<glm::mat4>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const std::vector<glm::dmat4>& value);
};

template <>
inline void Shader::set_uniform<int>(int location, int v0) {
	bind();
	glUniform1i(location, v0);
}

template <>
inline void Shader::set_uniform<unsigned int>(int location, unsigned int v0) {
	bind();
	glUniform1ui(location, v0);
}

template <>
inline void Shader::set_uniform<float>(int location, float v0) {
	bind();
	glUniform1f(location, v0);
}

template <>
inline void Shader::set_uniform<double>(int location, double v0) {
	bind();
	glUniform1d(location, v0);
}

template <>
inline void Shader::set_uniform<int>(const std::string& name, int v0) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0);
}

template <>
inline void Shader::set_uniform<unsigned int>(const std::string& name, unsigned int v0) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0);
}

template <>
inline void Shader::set_uniform<float>(const std::string& name, float v0) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0);
}

template <>
inline void Shader::set_uniform<double>(const std::string& name, double v0) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0);
}

template <>
inline void Shader::set_uniform<int>(int location, int v0, int v1) {
	bind();
	glUniform2i(location, v0, v1);
}

template <>
inline void Shader::set_uniform<unsigned int>(int location, unsigned int v0, unsigned int v1) {
	bind();
	glUniform2ui(location, v0, v1);
}

template <>
inline void Shader::set_uniform<float>(int location, float v0, float v1) {
	bind();
	glUniform2f(location, v0, v1);
}

template <>
inline void Shader::set_uniform<double>(int location, double v0, double v1) {
	bind();
	glUniform2d(location, v0, v1);
}

template <>
inline void Shader::set_uniform<int>(const std::string& name, int v0, int v1) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1);
}

template <>
inline void Shader::set_uniform<unsigned int>(const std::string& name, unsigned int v0, unsigned int v1) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1);
}

template <>
inline void Shader::set_uniform<float>(const std::string& name, float v0, float v1) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1);
}

template <>
inline void Shader::set_uniform<double>(const std::string& name, double v0, double v1) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1);
}

template <>
inline void Shader::set_uniform<int>(int location, int v0, int v1, int v2) {
	bind();
	glUniform3i(location, v0, v1, v2);
}

template <>
inline void Shader::set_uniform<unsigned int>(int location, unsigned int v0, unsigned int v1, unsigned int v2) {
	bind();
	glUniform3ui(location, v0, v1, v2);
}

template <>
inline void Shader::set_uniform<float>(int location, float v0, float v1, float v2) {
	bind();
	glUniform3f(location, v0, v1, v2);
}

template <>
inline void Shader::set_uniform<double>(int location, double v0, double v1, double v2) {
	bind();
	glUniform3d(location, v0, v1, v2);
}

template <>
inline void Shader::set_uniform<int>(const std::string& name, int v0, int v1, int v2) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1, v2);
}

template <>
inline void Shader::set_uniform<unsigned int>(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1, v2);
}

template <>
inline void Shader::set_uniform<float>(const std::string& name, float v0, float v1, float v2) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1, v2);
}

template <>
inline void Shader::set_uniform<double>(const std::string& name, double v0, double v1, double v2) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1, v2);
}

template <>
inline void Shader::set_uniform<int>(int location, int v0, int v1, int v2, int v3) {
	bind();
	glUniform4i(location, v0, v1, v2, v3);
}

template <>
inline void Shader::set_uniform<unsigned int>(int location, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) {
	bind();
	glUniform4ui(location, v0, v1, v2, v3);
}

template <>
inline void Shader::set_uniform<float>(int location, float v0, float v1, float v2, float v3) {
	bind();
	glUniform4f(location, v0, v1, v2, v3);
}

template <>
inline void Shader::set_uniform<double>(int location, double v0, double v1, double v2, double v3) {
	bind();
	glUniform4d(location, v0, v1, v2, v3);
}

template <>
inline void Shader::set_uniform<int>(const std::string& name, int v0, int v1, int v2, int v3) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1, v2, v3);
}

template <>
inline void Shader::set_uniform<unsigned int>(const std::string& name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1, v2, v3);
}

template <>
inline void Shader::set_uniform<float>(const std::string& name, float v0, float v1, float v2, float v3) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1, v2, v3);
}

template <>
inline void Shader::set_uniform<double>(const std::string& name, double v0, double v1, double v2, double v3) {
	int loc = get_uniform_location(name);
	set_uniform(loc, v0, v1, v2, v3);
}

template <>
inline void Shader::set_uniform<int>(int location, unsigned int count, unsigned int elements, const int *value) {
	if(location < 0)
		return;

	bind();

	switch(elements) {
	case 1:
		glUniform1iv(location, count, value);
		break;
	case 2:
		glUniform2iv(location, count, value);
		break;
	case 3:
		glUniform3iv(location, count, value);
		break;
	case 4:
		glUniform4iv(location, count, value);
		break;
	default:
		std::string error = "Wrong number of elements given to"
			"the set_uniform function";
		App::error_string.push_back(error);
		throw std::runtime_error(error);
		break;
	}
}

template <>
inline void Shader::set_uniform<unsigned int>(int location, unsigned int count, unsigned int elements, const unsigned int *value) {
	if(location < 0)
		return;

	bind();

	switch(elements) {
	case 1:
		glUniform1uiv(location, count, value);
		break;
	case 2:
		glUniform2uiv(location, count, value);
		break;
	case 3:
		glUniform3uiv(location, count, value);
		break;
	case 4:
		glUniform4uiv(location, count, value);
		break;
	default:
		std::string error = "Wrong number of elements given to"
			"the set_uniform function";
		App::error_string.push_back(error);
		throw std::runtime_error(error);
		break;
	}
}

template <>
inline void Shader::set_uniform<float>(int location, unsigned int count, unsigned int elements, const float *value) {
	if(location < 0)
		return;

	bind();

	switch(elements) {
	case 1:
		glUniform1fv(location, count, value);
		break;
	case 2:
		glUniform2fv(location, count, value);
		break;
	case 3:
		glUniform3fv(location, count, value);
		break;
	case 4:
		glUniform4fv(location, count, value);
		break;
	default:
		std::string error = "Wrong number of elements given to"
			"the set_uniform function";
		App::error_string.push_back(error);
		throw std::runtime_error(error);
		break;
	}
}

template <>
inline void Shader::set_uniform<double>(int location, unsigned int count, unsigned int elements, const double *value) {
	if(location < 0)
		return;

	bind();

	switch(elements) {
	case 1:
		glUniform1dv(location, count, value);
		break;
	case 2:
		glUniform2dv(location, count, value);
		break;
	case 3:
		glUniform3dv(location, count, value);
		break;
	case 4:
		glUniform4dv(location, count, value);
		break;
	default:
		std::string error = "Wrong number of elements given to"
			"the set_uniform function";
		App::error_string.push_back(error);
		throw std::runtime_error(error);
		break;
	}
}

template <>
inline void Shader::set_uniform<int>(const std::string& name, unsigned int count, unsigned int elements, const int *value) {

	int loc = get_uniform_location(name);
	set_uniform(loc, count, elements, value);
}

template <>
inline void Shader::set_uniform<unsigned int>(const std::string& name, unsigned int count, unsigned int elements, const unsigned int *value) {

	int loc = get_uniform_location(name);
	set_uniform(loc, count, elements, value);
}

template <>
inline void Shader::set_uniform<float>(const std::string& name, unsigned int count, unsigned int elements, const float *value) {

	int loc = get_uniform_location(name);
	set_uniform(loc, count, elements, value);
}

template <>
inline void Shader::set_uniform<double>(const std::string& name, unsigned int count, unsigned int elements, const double *value) {

	int loc = get_uniform_location(name);
	set_uniform(loc, count, elements, value);
}

template <>
inline void Shader::set_uniform<float>(int location, unsigned int count, unsigned int columns, unsigned int rows, bool transpose, const float *value) {
	if(location < 0)
		return;
	if(columns < 2 || rows < 2 || columns > 4 || rows > 4)
		return;

	bind();

	if(columns == rows) {
		switch(rows) {
		case 2:
			glUniformMatrix2fv(location,
				count,
				transpose,
				value);
			break;
		case 3:
			glUniformMatrix3fv(location,
				count,
				transpose,
				value);
			break;
		case 4:
			glUniformMatrix4fv(location,
				count,
				transpose,
				value);
			break;
		default:
			std::string error = "Wrong number of elements given to"
				"the set_uniform function";
			App::error_string.push_back(error);
			throw std::runtime_error(error);
			break;
		}
	} else {
		if(columns == 2 && rows == 3) {
			glUniformMatrix2x3fv(location, count, transpose, value);
		} else if(columns == 3 && rows == 2) {
			glUniformMatrix3x2fv(location, count, transpose, value);
		} else if(columns == 2 && rows == 4) {
			glUniformMatrix2x4fv(location, count, transpose, value);
		} else if(columns == 4 && rows == 2) {
			glUniformMatrix4x2fv(location, count, transpose, value);
		} else if(columns == 3 && rows == 4) {
			glUniformMatrix3x4fv(location, count, transpose, value);
		} else if(columns == 4 && rows == 3) {
			glUniformMatrix4x3fv(location, count, transpose, value);
		} else {
			std::string error = "Wrong number of elements given to"
				"the set_uniform function";
			App::error_string.push_back(error);
			throw std::runtime_error(error);
		}
	}
}

template <>
inline void Shader::set_uniform<double>(int location, unsigned int count, unsigned int columns, unsigned int rows, bool transpose, const double *value) {
	if(location < 0)
		return;
	if(columns < 2 || rows < 2 || columns > 4 || rows > 4)
		return;

	bind();

	if(columns == rows) {
		switch(rows) {
		case 2:
			glUniformMatrix2dv(location,
				count,
				transpose,
				value);
			break;
		case 3:
			glUniformMatrix3dv(location,
				count,
				transpose,
				value);
			break;
		case 4:
			glUniformMatrix4dv(location,
				count,
				transpose,
				value);
			break;
		default:
			std::string error = "Wrong number of elements given to"
				"the set_uniform function";
			App::error_string.push_back(error);
			throw std::runtime_error(error);
			break;

		}
	} else {
		if(columns == 2 && rows == 3) {
			glUniformMatrix2x3dv(location, count, transpose, value);
		} else if(columns == 3 && rows == 2) {
			glUniformMatrix3x2dv(location, count, transpose, value);
		} else if(columns == 2 && rows == 4) {
			glUniformMatrix2x4dv(location, count, transpose, value);
		} else if(columns == 4 && rows == 2) {
			glUniformMatrix4x2dv(location, count, transpose, value);
		} else if(columns == 3 && rows == 4) {
			glUniformMatrix3x4dv(location, count, transpose, value);
		} else if(columns == 4 && rows == 3) {
			glUniformMatrix4x3dv(location, count, transpose, value);
		} else {
			std::string error = "Wrong number of elements given to"
				"the set_uniform function";
			App::error_string.push_back(error);
			throw std::runtime_error(error);
		}
	}
}

template <>
inline void Shader::set_uniform<float>(const std::string& name, unsigned int count, unsigned int columns, unsigned int rows, bool transpose, const float *value) {

	int loc = get_uniform_location(name);
	set_uniform(loc, count, columns, rows, transpose, value);
}

template <>
inline void Shader::set_uniform<double>(const std::string& name, unsigned int count, unsigned int columns, unsigned int rows, bool transpose, const double *value) {

	int loc = get_uniform_location(name);
	set_uniform(loc, count, columns, rows, transpose, value);
}

}

#endif
