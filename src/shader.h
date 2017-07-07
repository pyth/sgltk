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

	std::map<std::string, GLenum> shader_path_map;

	std::map<std::string, GLenum> shader_string_map;

	static std::vector<std::string> paths;
public:
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
	 * @param buffer_mode The mode used to capture the variables when transform feedback is active. Must be must be GL_INTERLEAVED_ATTRIBS or GL_SEPARATE_ATTRIBS.
	 * @note This function needs to be called before you link the shader program
	 * @see link
	 */
	EXPORT void set_transform_feedback_variables(std::vector<char *>& variables, GLenum buffer_mode);
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
	EXPORT void set_uniform_int(int location, int v0);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value
	 */
	EXPORT void set_uniform_uint(int location, unsigned int v0);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value
	 */
	EXPORT void set_uniform_float(int location, float v0);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value
	 */
	EXPORT void set_uniform_int(const std::string& name, int v0);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value
	 */
	EXPORT void set_uniform_uint(const std::string& name, unsigned int v0);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value
	 */
	EXPORT void set_uniform_float(const std::string& name, float v0);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 */
	EXPORT void set_uniform_int(int location, int v0,
						  int v1);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 */
	EXPORT void set_uniform_uint(int location, unsigned int v0,
						   unsigned int v1);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 */
	EXPORT void set_uniform_float(int location, float v0,
						    float v1);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 */
	EXPORT void set_uniform_int(const std::string& name, int v0, int v1);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 */
	EXPORT void set_uniform_uint(const std::string& name, unsigned int v0,
							      unsigned int v1);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 */
	EXPORT void set_uniform_float(const std::string& name, float v0,
							       float v1);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 */
	EXPORT void set_uniform_int(int location, int v0,
						  int v1,
						  int v2);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 */
	EXPORT void set_uniform_uint(int location, unsigned int v0,
						   unsigned int v1,
						   unsigned int v2);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 */
	EXPORT void set_uniform_float(int location, float v0,
						    float v1,
						    float v2);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 */
	EXPORT void set_uniform_int(const std::string& name, int v0,
							     int v1,
							     int v2);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 */
	EXPORT void set_uniform_uint(const std::string& name, unsigned int v0,
							      unsigned int v1,
							      unsigned int v2);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 */
	EXPORT void set_uniform_float(const std::string& name, float v0,
							       float v1,
							       float v2);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 * @param v3 The value of the fourth element
	 */
	EXPORT void set_uniform_int(int location, int v0,
						  int v1,
						  int v2,
						  int v3);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 * @param v3 The value of the fourth element
	 */
	EXPORT void set_uniform_uint(int location, unsigned int v0,
						   unsigned int v1,
						   unsigned int v2,
						   unsigned int v3);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 * @param v3 The value of the fourth element
	 */
	EXPORT void set_uniform_float(int location, float v0,
						    float v1,
						    float v2,
						    float v3);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 * @param v3 The value of the fourth element
	 */
	EXPORT void set_uniform_int(const std::string& name, int v0,
							     int v1,
							     int v2,
							     int v3);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 * @param v3 The value of the fourth element
	 */
	EXPORT void set_uniform_uint(const std::string& name, unsigned int v0,
							      unsigned int v1,
							      unsigned int v2,
							      unsigned int v3);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param v0 The value of the first element
	 * @param v1 The value of the second element
	 * @param v2 The value of the third element
	 * @param v3 The value of the fourth element
	 */
	EXPORT void set_uniform_float(const std::string& name, float v0,
							       float v1,
							       float v2,
							       float v3);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param count The number of vectors
	 * @param elements The number of elements per vector
	 * @param value A pointer to the values
	 */
	EXPORT void set_uniform_int(int location,
				    unsigned int count,
				    unsigned int elements,
				    const int *value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param count The number of vectors
	 * @param elements The number of elements per vector
	 * @param value A pointer to the values
	 */
	EXPORT void set_uniform_uint(int location,
				     unsigned int count,
				     unsigned int elements,
				     const unsigned int *value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param count The number of vectors
	 * @param elements The number of elements per vector
	 * @param value A pointer to the values
	 */
	EXPORT void set_uniform_float(int location,
				      unsigned int count,
				      unsigned int elements,
				      const float *value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param count The number of vectors
	 * @param elements The number of elements per vector
	 * @param value A pointer to the values
	 */
	EXPORT void set_uniform_int(const std::string& name,
				    unsigned int count,
				    unsigned int elements,
				    const int *value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param count The number of vectors
	 * @param elements The number of elements per vector
	 * @param value A pointer to the values
	 */
	EXPORT void set_uniform_uint(const std::string& name,
				     unsigned int count,
				     unsigned int elements,
				     const unsigned int *value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param count The number of vectors
	 * @param elements The number of elements per vector
	 * @param value A pointer to the values
	 */
	EXPORT void set_uniform_float(const std::string& name,
				      unsigned int count,
				      unsigned int elements,
				      const float *value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param count The number of vectors
	 * @param columns The number of matrix columns
	 * @param rows The number of matrix rows
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A pointer to the values
	 */
	EXPORT void set_uniform(int location,
				unsigned int count,
				unsigned int columns,
				unsigned int rows,
				bool transpose,
				const float *value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param count The number of vectors
	 * @param columns The number of matrix columns
	 * @param rows The number of matrix rows
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A pointer to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				unsigned int count,
				unsigned int columns,
				unsigned int rows,
				bool transpose,
				const float *value);
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
	EXPORT void set_uniform(int location, const glm::vec3& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location, const glm::vec4& value);
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
				const glm::vec3& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const glm::vec4& value);
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
				const glm::mat3& value);
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
				const glm::mat3& value);
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
				const std::vector<glm::vec3>& value);
	/**
	 * @brief Sets the uniform
	 * @param location The uniform location
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(int location,
				const std::vector<glm::vec4>& value);
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
				const std::vector<glm::vec3>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				const std::vector<glm::vec4>& value);
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
				const std::vector<glm::mat3>& value);
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
				const std::vector<glm::mat3>& value);
	/**
	 * @brief Sets the uniform
	 * @param name The uniform name
	 * @param transpose Specifies whether to transpose the matrix
	 * @param value A reference to the values
	 */
	EXPORT void set_uniform(const std::string& name,
				bool transpose,
				const std::vector<glm::mat4>& value);
};

}

#endif
