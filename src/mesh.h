#ifndef __MESH_H__
#define __MESH_H__

#include "app.h"
#include "buffer.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"

namespace sgltk {

/**
 * @struct Vertex
 * @brief A basic vertex structure
 */
typedef EXPORT struct Vertex {
	/**
	 * @brief Vertex position
	 */
	glm::vec4 position;
	/**
	 * @brief Vertex normal
	 */
	glm::vec3 normal;
	/**
	 * @brief Vertex tangent
	 */
	glm::vec4 tangent;
	/**
	 * @brief Vertex bi-tangent
	 */
	glm::vec4 bitangent;
	/**
	 * @brief Vertex color
	 */
	glm::vec4 color;
	/**
	 * @brief Vertex texture coordinates
	 */
	glm::vec3 tex_coord;

	Vertex() {
		position = {0, 0, 0, 1};

		normal = {0, 0, 0};

		tangent = {0, 0, 0, 1};

		bitangent = glm::vec4(glm::cross(normal, glm::vec3(tangent)), 1);

		color = {0, 0, 0, 0};
	};

	/**
	 * @param p	Vertex position
	 * @param n	Vertex normal
	 */
	Vertex(glm::vec3 p, glm::vec3 n) {
		position = glm::vec4(p, 1);

		normal = n;

		tangent = {0, 0, 0, 1};

		bitangent = glm::vec4(glm::cross(normal, glm::vec3(tangent)), 1);

		color = {0, 0, 0, 0};

		tex_coord = {0, 0, 0};
	};

	/**
	 * @param p	Vertex position
	 * @param n	Vertex normal
	 * @param tc	Texture coordinates
	 */
	Vertex(glm::vec3 p, glm::vec3 n, glm::vec3 tc) {
		position = glm::vec4(p, 1);

		normal = n;

		tangent = {0, 0, 0, 1};

		bitangent = glm::vec4(glm::cross(normal, glm::vec3(tangent)), 1);

		color = {0, 0, 0, 0};

		tex_coord = tc;
	};

	/**
	 * @param p	Vertex position
	 * @param n	Vertex normal
	 * @param t	Vertex tangent
	 * @param tc	Texture coordinates
	 */
	Vertex(glm::vec3 p, glm::vec3 n, glm::vec3 t, glm::vec3 tc) {
		position = glm::vec4(p, 1);

		normal = n;

		tangent = glm::vec4(t, 1);

		bitangent = glm::vec4(glm::cross(normal, glm::vec3(tangent)), 1);

		color = {0, 0, 0, 0};

		tex_coord = tc;
	};

	/**
	 * @param p	Vertex position
	 * @param n	Vertex normal
	 * @param t	Vertex tangent
	 * @param c	Vertex color
	 * @param tc	Texture coordinates
	 */
	Vertex(glm::vec3 p, glm::vec3 n, glm::vec3 t,
	       glm::vec4 c, glm::vec3 tc) {
		position = glm::vec4(p, 1);

		normal = n;

		tangent = glm::vec4(t, 1);

		bitangent = glm::vec4(glm::cross(normal, glm::vec3(tangent)), 1);

		color = c;

		tex_coord = tc;
	};
} Vertex;


/**
 * @class Mesh
 * @brief Manages meshes
 */
class Mesh {
	GLuint vao;
	GLenum tf_mode;

	glm::mat4 *view_matrix;
	glm::mat4 *projection_matrix;

	std::vector<std::unique_ptr<Buffer> > vbo;

	GLenum index_type;
	std::vector<std::unique_ptr<Buffer> > ibo;

	std::vector<Buffer*> attached_buffers;
	std::vector<GLuint> attached_buffers_targets;
	std::vector<unsigned int> attached_buffers_indices;

	void material_uniform();
public:
	/**
	 * @brief Number of texture coordinates
	 */
	unsigned int num_uv;
	/**
	 * @brief Number of vertex colors
	 */
	unsigned int num_col;
	/**
	 * @brief Number of vertices
	 */
	unsigned int num_vertices;
	/**
	 * @brief The name of the ambient materiel component
	 */
	std::string ambient_color_name;
	/**
	 * @brief The name of the diffuse materiel component
	 */
	std::string diffuse_color_name;
	/**
	 * @brief The name of the specular materiel component
	 */
	std::string specular_color_name;
	/**
	 * @brief The name of the specular factor
	 */
	std::string shininess_name;
	/**
	 * @brief The name of the specular exponent
	 */
	std::string shininess_strength_name;

	/**
	 * @brief The name of the model matrix in the shader
	 */
	std::string model_matrix_name;
	/**
	 * @brief The name of the view matrix in the shader
	 */
	std::string view_matrix_name;
	/**
	 * @brief The name of the projection matrix in the shader
	 */
	std::string projection_matrix_name;
	/**
	 * @brief The name of the model-view matrix in the shader
	 */
	std::string model_view_matrix_name;
	/**
	 * @brief The name of the view-projection matrix in the shader
	 */
	std::string view_proj_matrix_name;
	/**
	 * @brief The name of the model-view-projection matrix in the shader
	 */
	std::string model_view_projection_matrix_name;
	/**
	 * @brief The name of the normal matrix in the shader
	 */
	std::string normal_matrix_name;

	/**
	 * @brief The shader being used to draw the mesh
	 */
	Shader *shader;
	/**
	 * @brief The bounding box
	 */
	std::vector<glm::vec3> bounding_box;
	/**
	 * @brief The model matrix
	 */
	glm::mat4 model_matrix;
	/**
	 * @brief The shininess of the material
	 */
	float shininess;
	/**
	 * @brief The strength of the shininess of the material
	 */
	float shininess_strength;
	/**
	 * @brief The ambient color component of the material
	 */
	glm::vec4 color_ambient;
	/**
	 * @brief The diffuse color component of the material
	 */
	glm::vec4 color_diffuse;
	/**
	 * @brief The specular color component of the material
	 */
	glm::vec4 color_specular;
	/**
	 * @brief Attached textures
	 */
	std::vector<std::tuple<std::string, const Texture&, unsigned int> > auto_textures;
	/**
	 * @brief Attached user textures
	 */
	std::vector<std::tuple<std::string, const Texture&, unsigned int> > textures;

	/**
	 * @brief Indicates that the mesh should be drawn as a wireframe
	 */
	bool wireframe;
	/**
	 * @brief Indicates that the mesh should not be drawn using
	 * 	  back face culling
	 */
	bool twosided;

	EXPORT Mesh();
	EXPORT ~Mesh();

	/**
	 * @brief Specifies the shader to use to render the mesh
	 * @param shader The shader to be used to render the mesh
	 */
	EXPORT void setup_shader(Shader *shader);
	/**
	 * @brief Sets up the view and projection matrices that will be used
	 * 	  by the mesh
	 * @param view_matrix The view matrix
	 * @param projection_matrix The projection matrix
	 * @return Returns true if both pointers are not nullptr, flase otherwise
	 */
	EXPORT bool setup_camera(glm::mat4 *view_matrix,
				 glm::mat4 *projection_matrix);
	/**
	 * @brief Sets up the view and projection matrices that will be used
	 * 	  by the mesh
	 * @param camera The camera to use
	 * @return Returns true on success, false otherwise
	 */
	EXPORT bool setup_camera(Camera *camera);
	/**
	 * @brief Sets the name of the model matrix in the shader
	 * @param name The name of the model matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "model_matrix"
	 */
	EXPORT void set_model_matrix_name(const std::string& name);
	/**
	 * @brief Sets the name of the view matrix in the shader
	 * @param name The name of the view matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "view_matrix"
	 */
	EXPORT void set_view_matrix_name(const std::string& name);
	/**
	 * @brief Sets the name of the projection matrix in the shader
	 * @param name The name of the projection matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "proj_matrix"
	 */
	EXPORT void set_projection_matrix_name(const std::string& name);
	/**
	 * @brief Sets the name of the model-view matrix in the shader
	 * @param name The name of the model-view matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "model_view_matrix"
	 */
	EXPORT void set_model_view_matrix_name(const std::string& name);
	/**
	 * @brief Sets the name of the view-projection matrix in the shader
	 * @param name The name of the view-projection matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "view_proj_matrix"
	 */
	EXPORT void set_view_proj_matrix_name(const std::string& name);
	/**
	 * @brief Sets the name of the model-view-projection matrix
	 * 	in the shader
	 * @param name The name of the model-view-projection matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "model_view_proj_matrix"
	 */
	EXPORT void set_model_view_proj_name(const std::string& name);
	/**
	 * @brief Sets the name of the normal matrix in the shader
	 * @param name The name of the normal matrix.
	 * 	The name is reset if string is empty.
	 * @note Default value is "normal_matrix"
	 */
	EXPORT void set_normal_matrix_name(const std::string& name);
	/**
	 * @brief Sets the name of the ambient color in the shader
	 * @param name The name of the ambient color component.
	 * 	The name is reset if string is empty.
	 * @note Default value is "color_ambient"
	 */
	EXPORT void set_ambient_color_name(const std::string& name);
	/**
	 * @brief Sets the name of the diffuse color in the shader
	 * @param name The name of the diffuse color component.
	 * 	The name is reset if string is empty.
	 * @note Default value is "color_diffuse"
	 */
	EXPORT void set_diffuse_color_name(const std::string& name);
	/**
	 * @brief Sets the name of the specular color in the shader
	 * @param name The name of the specular color component.
	 * 	The name is reset if string is empty.
	 * @note Default value is "color_specular"
	 */
	EXPORT void set_specular_color_name(const std::string& name);
	/**
	 * @brief Sets the name of the shininess of the material
	 * 	in the shader
	 * @param name The name of the shininess component.
	 * 	The name is reset if string is empty.
	 * @note Default value is "shininess"
	 */
	EXPORT void set_shininess_name(const std::string& name);
	/**
	 * @brief Sets the name of the shininess strength of the material
	 * 	in the shader
	 * @param name The name of the shininess strength component.
	 * 	The name is reset if string is empty.
	 * @note Default value is "shininess_strength"
	 */
	EXPORT void set_shininess_strength_name(const std::string& name);
	/**
	 * @brief Attaches a texture to the mesh
	 * @param name The name of the texture in the shader
	 * @param texture The texture to attach
	 * @param index The index of the texture in the uniform array
	 */
	EXPORT void attach_texture(const std::string& name,
				   const sgltk::Texture& texture,
				   unsigned int index = 0);
	/**
	 * @brief Sets the output type for transform feedback operations
	 * @param mode The output type of the primitives that will be recorded
	 * 	into the buffer objects that are bound for transform feedback
	 */
	EXPORT void set_transform_feedback_mode(GLenum mode);
	/**
	 * @brief Attaches a buffer that is automatically bound before
	 * 	each draw call
	 * @param buffer The buffer to attach_buffer
	 * @param target The target the buffer will be bound to
	 * @param index The index of the binding point within the array
	 * 		specified by target.
	 * @note If the target of the buffer is not GL_ATOMIC_COUNTER_BUFFER,
	 * 	 GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER or
	 * 	 GL_SHADER_STORAGE_BUFFER the index is ignored.
	 */
	EXPORT void attach_buffer(const sgltk::Buffer *buffer, GLuint target, unsigned int index = 0);
	/**
	 * @brief Loads data into memory
	 * @param vertexdata The data to be loaded into memory
	 * @param number_elements Number of elements
	 * @param usage A hint as to how the buffer will be accessed.
	 * 	Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 * @return Returns the index of the buffer in the list of all attached
	 * 	vertex buffers
	 */
	template <typename T = Vertex>
	unsigned int attach_vertex_buffer(const void *vertexdata,
					  unsigned int number_elements,
					  GLenum usage = GL_STATIC_DRAW);
	/**
	 * @brief Loads data into memory
	 * @param vertexdata The data to be loaded into memory
	 * @param usage A hint as to how the buffer will be accessed.
	 * 	Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 * @return Returns the index of the buffer in the list of all attached
	 * 	vertex buffers
	 */
	template <typename T = Vertex>
	unsigned int attach_vertex_buffer(const std::vector<T>& vertexdata,
					  GLenum usage = GL_STATIC_DRAW);
	/**
	 * @brief Overwrites all data in a vertex buffer
	 * @param buffer_index The index of the buffer to be modified
	 * @param data The data to be loaded into the buffer
	 * @param number_elements Number of elements
	 * @return Returns true on success, flase otherwise
	 */
	template <typename T = Vertex>
	bool replace_buffer_data(unsigned int buffer_index,
				 const void *data,
				 unsigned int number_elements);
	/**
	 * @brief Overwrites all data in a vertex buffer
	 * @param buffer_index The index of the buffer to be modified
	 * @param data The data to be loaded into the buffer
	 * @return Returns true on success, flase otherwise
	 */
	template <typename T = Vertex>
	bool replace_buffer_data(unsigned int buffer_index,
				 const std::vector<T>& data);
	/**
	 * @brief Modifies the data in a vertex buffer
	 * @param buffer_index The index of the buffer to be modified
	 * @param offset The byte offset into the buffer
	 * @param data The data to be loaded into the buffer
	 * @param number_elements Number of elements
	 * @return Returns true on success, flase otherwise
	 */
	template <typename T = Vertex>
	bool replace_partial_data(unsigned int buffer_index,
				  unsigned int offset,
				  const void *data,
				  unsigned int number_elements);
	/**
	 * @brief Modifies the data in a vertex buffer
	 * @param buffer_index The index of the buffer to be modified
	 * @param offset The byte offset into the buffer
	 * @param data The data to be loaded into the buffer
	 * @return Returns true on success, flase otherwise
	 */
	template <typename T = Vertex>
	bool replace_partial_data(unsigned int buffer_index,
				  unsigned int offset,
				  const std::vector<T>& data);
	/**
	 * @brief This is a convenience function that combines attach_vertex_buffer and
	 * 		set_vertex_attribute.
	 * @param attrib_name		The attribute name in the shader
	 * @param number_elements	Number of elements
	 * @param type			Element type
	 * @param data			The vertices to be loaded into memory
	 * @param usage A hint as to how the buffer will be accessed.
	 * 	Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 * @return	Returns 0 on success, -1 if no shader was
	 * 		specified for the mesh, -2 if the vertex attribute
	 * 		could not be found
	 */
	template <typename T = glm::vec3>
	int add_vertex_attribute(std::string attrib_name,
				 GLint number_elements,
				 GLenum type,
				 const void *data,
				 GLenum usage = GL_STATIC_DRAW);
	/**
	 * @brief This is a convenience function that combines attach_vertex_buffer and
	 * 		set_vertex_attribute.
	 * @param attrib_name		The attribute name in the shader
	 * @param number_elements	Number of elements
	 * @param type			Element type
	 * @param data			The vertices to be loaded into memory
	 * @param usage A hint as to how the buffer will be accessed.
	 * 	Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 * @return	Returns 0 on success, -1 if no shader was
	 * 		specified for the mesh, -2 if the vertex attribute
	 * 		could not be found
	 */
	template <typename T = glm::vec3>
	int add_vertex_attribute(std::string attrib_name,
				 GLint number_elements,
				 GLenum type,
				 const std::vector<T>& data,
				 GLenum usage = GL_STATIC_DRAW);
	/**
	 * @brief This is a convenience function that combines attach_vertex_buffer and
	 * 		set_vertex_attribute.
	 * @param attrib_location	The attribute location in the shader
	 * @param number_elements	Number of elements
	 * @param type			Element type
	 * @param data			The vertices to be loaded into memory
	 * @param usage A hint as to how the buffer will be accessed.
	 * 	Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 * @return	Returns 0 on success, -1 if no shader was
	 * 		specified for the mesh, -2 if the vertex attribute
	 * 		could not be found
	 */
	template <typename T = glm::vec3>
	int add_vertex_attribute(int attrib_location,
				 GLint number_elements,
				 GLenum type,
				 const void *data,
				 GLenum usage = GL_STATIC_DRAW);
	/**
	 * @brief This is a convenience function that combines attach_vertex_buffer and
	 * 		set_vertex_attribute.
	 * @param attrib_location	The attribute location in the shader
	 * @param number_elements	Number of elements
	 * @param type			Element type
	 * @param data			The vertices to be loaded into memory
	 * @param usage A hint as to how the buffer will be accessed.
	 * 	Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 * @return	Returns 0 on success, -1 if no shader was
	 * 		specified for the mesh, -2 if the vertex attribute
	 * 		could not be found
	 */
	template <typename T = glm::vec3>
	int add_vertex_attribute(int attrib_location,
				 GLint number_elements,
				 GLenum type,
				 const std::vector<T>& data,
				 GLenum usage = GL_STATIC_DRAW);
	 /**
	 * @brief Sets pointers to vertex attributes
	 * @param attrib_name		The attribute name in the shader
	 * @param buffer		The buffer that contains the attribute
	 * @param number_elements	Number of elements
	 * @param type			Element type
	 * @param stride		Memory offset between vertices
	 * @param pointer		The offset of the attribute in the
	 * 				vertex structure
	 * @param divisor Determines how many instances share the same attribute value.
	 * 		0 means that every shader instance gets a new value,
	 * 		1 means every mesh instance gets a new value,
	 * 		2 means that two instances get the same value and so on.
	 * @return	Returns 0 on success, -1 if no shader was
	 * 		specified for the mesh, -2 if the vertex attribute
	 * 		could not be found
	 */
	EXPORT int set_buffer_vertex_attribute(const std::string& attrib_name,
				        sgltk::Buffer *buffer,
				        GLint number_elements,
				        GLenum type,
				        GLsizei stride,
				        const GLvoid *pointer,
				        unsigned int divisor = 0);
	/**
	 * @brief Sets pointers to vertex attributes
	 * @param attrib_location	The attribute location in the shader
	 * @param buffer		The buffer that contains the attribute
	 * @param number_elements	Number of elements
	 * @param type			Element type
	 * @param stride		Memory offset between vertices
	 * @param pointer		The offset of the attribute in the
	 * 				vertex structure
	 * @param divisor Determines how many instances share the same attribute value.
	 * 		0 means that every shader instance gets a new value,
	 * 		1 means every mesh instance gets a new value,
	 * 		2 means that two instances get the same value and so on.
	 * @return	Returns 0 on success, -1 if no shader was
	 * 		specified for the mesh, -2 if the vertex attribute
	 * 		could not be found
	 */
	EXPORT int set_buffer_vertex_attribute(int attrib_location,
					sgltk::Buffer *buffer,
					GLint number_elements,
					GLenum type,
					GLsizei stride,
					const GLvoid *pointer,
					unsigned int divisor = 0);
	/**
	 * @brief Sets pointers to vertex attributes
	 * @param attrib_name		The attribute name in the shader
	 * @param buffer_index		The index of the buffer that contains
	 *				the attribute
	 * @param number_elements	Number of elements
	 * @param type			Element type
	 * @param stride		Memory offset between vertices
	 * @param pointer		The offset of the attribute in the
	 * 				vertex structure
	 * @param divisor Determines how many instances share the same attribute value.
	 * 		0 means that every shader instance gets a new value,
	 * 		1 means every mesh instance gets a new value,
	 * 		2 means that two instances get the same value and so on.
	 * @return	Returns 0 on success, -1 if no shader was
	 * 		specified for the mesh, -2 if the vertex attribute
	 * 		could not be found
	 */
	EXPORT int set_vertex_attribute(const std::string& attrib_name,
				        unsigned int buffer_index,
				        GLint number_elements,
				        GLenum type,
				        GLsizei stride,
				        const GLvoid *pointer,
				        unsigned int divisor = 0);
	/**
	 * @brief Sets pointers to vertex attributes
	 * @param attrib_location	The attribute location in the shader
	 * @param buffer_index		The index of the buffer that contains
	 *				the attribute
	 * @param number_elements	Number of elements
	 * @param type			Element type
	 * @param stride		Memory offset between vertices
	 * @param pointer		The offset of the attribute in the
	 * 				vertex structure
	 * @param divisor Determines how many instances share the same attribute value.
	 * 		0 means that every shader instance gets a new value,
	 * 		1 means every mesh instance gets a new value,
	 * 		2 means that two instances get the same value and so on.
	 * @return	Returns 0 on success, -1 if no shader was
	 * 		specified for the mesh, -2 if the vertex attribute
	 * 		could not be found
	 */
	EXPORT int set_vertex_attribute(int attrib_location,
				        unsigned int buffer_index,
				        GLint number_elements,
				        GLenum type,
				        GLsizei stride,
				        const GLvoid *pointer,
				        unsigned int divisor = 0);
	/**
	 * @brief Attaches an index array to the mesh
	 * @param indices Indices describing the topology of the mesh
	 * @return Returns the index of the index-buffer or -1 on failure.
	 * 	   This function will fail if the index type does not match that
	 * 	   of an already attached buffer
	 * @note You can attach multiple index arrays
	 */
	EXPORT int attach_index_buffer(const std::vector<unsigned char>& indices);
	/**
	 * @brief Attaches an index array to the mesh
	 * @param indices Indices describing the topology of the mesh
	 * @return Returns the index of the index-buffer or -1 on failure.
	 * 	   This function will fail if the index type does not match that
	 * 	   of an already attached buffer
	 * @note You can attach multiple index arrays
	 */
	EXPORT int attach_index_buffer(const std::vector<unsigned short>& indices);
	/**
	 * @brief Attaches an index array to the mesh
	 * @param indices Indices describing the topology of the mesh
	 * @return Returns the index of the index-buffer or -1 on failure.
	 * 	   This function will fail if the index type does not match that
	 * 	   of an already attached buffer
	 * @note You can attach multiple index arrays
	 */
	EXPORT int attach_index_buffer(const std::vector<unsigned int>& indices);

	/**
	 * @brief Sets a buffer to write the output of the vertex shader to
	 * @param buffer The buffer to write to
	 * @param size The size of the buffer in bytes
	 * @param usage A hint as to how the buffer will be accessed.
	 * 	Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 */
	EXPORT void attach_tf_buffer(void *buffer, unsigned int size, GLenum usage = GL_STATIC_READ);

	/**
	 * @brief Computes the bounding box of the mesh
	 * @param vertexdata The vertices of the mesh
	 * @param pointer The pointer to the position vector in the vertex
	 * 	structure
	 */
	template <typename T = Vertex>
	void compute_bounding_box(const std::vector<T>& vertexdata, unsigned int pointer);

	/**
	 * @brief Renders the mesh using the first index buffer
	 * @param mode Specifies the primitive that will be created from
	 * 	vertices
	 */
	EXPORT void draw(GLenum mode);

	/**
	 * @brief Renders the mesh using the first index buffer
	 * @param mode Specifies the primitive that will be created from
	 * 	  vertices
	 * @param model_matrix The model matrix to use
	 *	  (nullptr to use the model_matrix member)
	 */
	EXPORT void draw(GLenum mode, const glm::mat4 *model_matrix);

	/**
	 * @brief Renders the mesh
	 * @param mode Specifies the primitive that will be created from
	 * 	  vertices
	 * @param index_buffer The index buffer to use
	 * @param model_matrix The model matrix to use
	 *	  (nullptr to use the model_matrix member)
	 */
	EXPORT void draw(GLenum mode, unsigned int index_buffer,
					const glm::mat4 *model_matrix);

	/**
	 * @brief Renders the mesh multiple times
	 * @param mode Specifies the primitive that will be created from
	 * 	  vertices
	 * @param num_instances Number of instances to be drawn
	 */
	EXPORT void draw_instanced(GLenum mode, unsigned int num_instances);

	/**
	 * @brief Renders the mesh multiple times
	 * @param mode Specifies the primitive that will be created from
	 * 	  vertices
	 * @param index_buffer The index buffer to use
	 * @param num_instances Number of instances to be drawn
	 */
	EXPORT void draw_instanced(GLenum mode, unsigned int index_buffer,
						unsigned int num_instances);
};

template <typename T>
int Mesh::add_vertex_attribute(std::string attrib_name,
			       GLint number_elements,
			       GLenum type,
			       const void *data,
			       GLenum usage) {

	T *ptr = (T *)data;
	std::vector<T> tmp(ptr, ptr + number_elements);
	unsigned int buffer_index = attach_vertex_buffer<T>(tmp, usage);
	return set_vertex_attribute(attrib_name, buffer_index, number_elements,
					type, 0, 0);
}

template <typename T>
int Mesh::add_vertex_attribute(std::string attrib_name,
			       GLint number_elements,
			       GLenum type,
			       const std::vector<T>& data,
			       GLenum usage) {

	unsigned int buffer_index = attach_vertex_buffer<T>(data, usage);
	return set_vertex_attribute(attrib_name, buffer_index, number_elements,
					type, 0, 0);
}

template <typename T>
int Mesh::add_vertex_attribute(int attrib_location,
			       GLint number_elements,
			       GLenum type,
			       const void *data,
			       GLenum usage) {

	T *ptr = (T *)data;
	std::vector<T> tmp(ptr, ptr + number_elements);
	unsigned int buffer_index = attach_vertex_buffer<T>(tmp, usage);
	return set_vertex_attribute(attrib_location, buffer_index, number_elements,
					type, 0, 0);
}

template <typename T>
int Mesh::add_vertex_attribute(int attrib_location,
			       GLint number_elements,
			       GLenum type,
			       const std::vector<T>& data,
			       GLenum usage) {

	unsigned int buffer_index = attach_vertex_buffer<T>(data, usage);
	return set_vertex_attribute(attrib_location, buffer_index, number_elements,
					type, 0, 0);
}

template <typename T>
unsigned int Mesh::attach_vertex_buffer(const void *vertexdata,
					unsigned int number_elements,
					GLenum usage) {

	T *ptr = (T *)vertexdata;
	std::vector<T> tmp(ptr, ptr + number_elements);
	return attach_vertex_buffer<T>(tmp, usage);
}

template <typename T>
unsigned int Mesh::attach_vertex_buffer(const std::vector<T>& vertexdata,
					GLenum usage) {

	std::unique_ptr<Buffer> buf = std::make_unique<Buffer>(GL_ARRAY_BUFFER);
	buf->load<T>(vertexdata, usage);
	vbo.push_back(std::move(buf));

	return vbo.size() - 1;
}

template <typename T>
bool Mesh::replace_buffer_data(unsigned int buffer_index,
			       const void *data,
			       unsigned int number_elements) {

	T *ptr = (T *)data;
	std::vector<T> tmp(ptr, ptr + number_elements);
	return replace_buffer_data<T>(buffer_index, tmp);
}

template <typename T>
bool Mesh::replace_buffer_data(unsigned int buffer_index,
			       const std::vector<T>& data) {

	if(buffer_index >= vbo.size()) {
		App::error_string.push_back("The value of the variable"
				"buffer_index if greater than the number"
				"of vertex buffers.");
		return false;
	}

	vbo[buffer_index]->replace_data(data);

	return true;
}

template <typename T>
bool Mesh::replace_partial_data(unsigned int buffer_index,
				unsigned int offset,
				const void *data,
				unsigned int number_elements) {

	T *ptr = (T *)data;
	std::vector<T> tmp(ptr, ptr + number_elements);
	return replace_partial_data<T>(buffer_index, offset, tmp);
}

template <typename T>
bool Mesh::replace_partial_data(unsigned int buffer_index,
				unsigned int offset,
				const std::vector<T>& data) {

	if(buffer_index >= vbo.size()) {
		App::error_string.push_back("The value of the variable"
				"buffer_index if greater than the number"
				"of vertex buffers.");
		return false;
	}

	return vbo[buffer_index]->replace_partial_data(offset, data);
}

template <typename T>
void Mesh::compute_bounding_box(const std::vector<T>& vertexdata, unsigned int pointer) {
	glm::vec3 *pos = (glm::vec3 *)(&(vertexdata)[0] + pointer);
	bounding_box[0] = *pos;
	bounding_box[1] = *pos;
	for(size_t i = 1; i < vertexdata.size(); i++) {
		pos = (glm::vec3 *)(&(vertexdata)[i] + pointer);
		if(pos->x < bounding_box[0].x)
			bounding_box[0].x = pos->x;
		if(pos->y < bounding_box[0].y)
			bounding_box[0].y = pos->y;
		if(pos->z < bounding_box[0].z)
			bounding_box[0].z = pos->z;
		if(pos->x > bounding_box[1].x)
			bounding_box[1].x = pos->x;
		if(pos->y > bounding_box[1].y)
			bounding_box[1].y = pos->y;
		if(pos->z > bounding_box[1].z)
			bounding_box[1].z = pos->z;
	}
}
}

#endif
