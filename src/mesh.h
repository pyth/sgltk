#ifndef __MESH_H__
#define __MESH_H__

#include "core.h"
#include "shader.h"

/**
 * @brief A basic vertex structure 
 */
struct Vertex {
	/**
	 * @brief Vertex position
	 */
	GLfloat position[4];
	/**
	 * @brief Vertex normal
	 */
	GLfloat normal[3];
	/**
	 * @brief Vertex tangent
	 */
	GLfloat tangent[4];
	/**
	 * @brief Vertex bitangent
	 */
	GLfloat bitangent[4];
	/**
	 * @brief Vertex color
	 */
	GLfloat color[4];
	/**
	 * @brief Vertex texture coordinates
	 */
	GLfloat texcoord[3];

	Vertex() {
		position[0] = 0;
		position[1] = 0;
		position[2] = 0;
		position[3] = 0;

		normal[0] = 0;
		normal[1] = 0;
		normal[2] = 0;

		tangent[0] = 0;
		tangent[1] = 0;
		tangent[2] = 0;
		tangent[3] = 0;

		bitangent[0] = 0;
		bitangent[1] = 0;
		bitangent[2] = 0;
		bitangent[3] = 0;

		color[0] = 0;
		color[1] = 0;
		color[2] = 0;
		color[3] = 0;

		texcoord[0] = 0;
		texcoord[1] = 0;
		texcoord[2] = 0;
	};

	/**
	 * @param p	Vertex position
	 * @param n	Vertex normal
	 */
	Vertex(glm::vec3 p, glm::vec3 n) {
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;

	};

	/**
	 * @param p	Vertex position
	 * @param n	Vertex normal
	 * @param tc	Texture coordinates
	 */
	Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 tc) {
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;

		texcoord[0] = tc.x;
		texcoord[1] = tc.y;
	};

	/**
	 * @param p	Vertex position
	 * @param n	Vertex normal
	 * @param t	Vertex tangent
	 * @param tc	Texture coordinates
	 */
	Vertex(glm::vec3 p, glm::vec3 n, glm::vec3 t, glm::vec2 tc) {
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;

		tangent[0] = t.x;
		tangent[1] = t.y;
		tangent[2] = t.z;
		tangent[3] = 1;

		texcoord[0] = tc.x;
		texcoord[1] = tc.y;
	};

	/**
	 * @param p	Vertex position
	 * @param n	Vertex normal
	 * @param t	Vertex tangent
	 * @param c	Vertex color
	 * @param tc	Texture coordinates
	 */
	Vertex(glm::vec3 p, glm::vec3 n, glm::vec3 t, glm::vec4 c, glm::vec2 tc) {
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;

		tangent[0] = t.x;
		tangent[1] = t.y;
		tangent[2] = t.z;
		tangent[3] = 1;

		color[0] = c.x;
		color[1] = c.y;
		color[2] = c.z;
		color[3] = c.w;

		texcoord[0] = tc.x;
		texcoord[1] = tc.y;
	};
};

/**
 * @class Mesh
 * @brief Manages meshes
 */
class Mesh {
	Shader *shader;
	const char *model_view_matrix_name;
	const char *model_view_projection_matrix_name;
	const char *normal_matrix_name;
	glm::mat4 *view_matrix;
	glm::mat4 *projection_matrix;
	GLuint vao;
	GLuint vbo;
	std::vector<GLuint> ibo;
	std::vector<int> num_indices;
public:
	/**
	 * @brief The model matrix
	 */
	glm::mat4 model_matrix;

	Mesh();
	~Mesh();

	/**
	 * @brief Specifies the shader to use to render the mesh
	 * @param shader The shader to be used to render the mesh
	 * @param model_view_matrix_name The name of the model-view
	 *	matrixin in the shader
	 * @param model_view_projection_matrix_name The name of the
	 *	model-view-projection matrixin in the shader
	 * @param normal_matrix_name The name of the normal matrix
	 *	in the shader
	 * @param view_matrix The view matrix
	 * @param projection_matrix The projection matrix
	 */
	void setup_shader(Shader *shader, const char *model_view_matrix_name,
		const char *model_view_projection_matrix_name,
		const char *normal_matrix_name, glm::mat4 *view_matrix,
		glm::mat4 *projection_matrix);
	/**
	 * @brief Loads vertices into memory
	 * @param vertexdata The vertices to be loaded into memory
	 */
	void attach_vertex_array(const std::vector<Vertex> *vertexdata);
	/**
	 * @brief Sets pointers to vertex attribures
	 * @param attrib_name	The name as defined in the shader
	 * @param size		Number of elements
	 * @param type		Element type
	 * @param stride	Memory offset between vertices
	 * @param pointer	The offset of the attribute in the
	 *				vertex structure
	 */
	void set_vertex_attribute(const char *attrib_name, GLint size,
				  GLenum type, GLsizei stride,
				  const GLvoid *pointer);
	/**
	 * @brief Attaches an index array to the mesh
	 * @param indices Indices describing the topology of the mesh
	 * You can attach multiple index arrays
	 */
	void attach_index_array(const std::vector<unsigned short> *indices);

	/**
	 * @brief Renders the mesh using the first index buffer
	 * @param mode Specifies the primitive that will be created from
	 * 	vertices
	 */
	void draw(GLenum mode);

	/**
	 * @brief Renders the mesh using the first index buffer and the
	 * 	  model_matrix member as model matrix
	 * @param mode Specifies the primitive that will be created from
	 * 	  vertices
	 * @param index_buffer The index buffer to use
	 */
	void draw(GLenum mode, unsigned int index_buffer);

	/**
	 * @brief Renders the mesh using the first index buffer
	 * @param mode Specifies the primitive that will be created from
	 * 	  vertices
	 * @param model_matrix The model matrix to use
	 *	  (NULL to use the model_matrix member)
	 */
	void draw(GLenum mode, glm::mat4 *model_matrix);

	/**
	 * @brief Renders the mesh
	 * @param mode Specifies the primitive that will be created from
	 * 	  vertices
	 * @param index_buffer The index buffer to use
	 * @param model_matrix The model matrix to use
	 *	  (NULL to use the model_matrix member)
	 */
	void draw(GLenum mode, unsigned int index_buffer,
		  glm::mat4 *model_matrix);
};

#endif
