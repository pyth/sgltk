#ifndef __MESH_H__
#define __MESH_H__

#include "core.h"
#include "shader.h"

namespace sgltk {
	/**
	 * @brief A basic vertex structure 
	 */
	typedef struct Vertex {
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
		 * @brief Vertex bitangent
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

			bitangent = {0, 0, 0, 1};

			color = {0, 0, 0, 0};

			tex_coord = {0, 0, 0};
		};

		/**
		 * @param p	Vertex position
		 * @param n	Vertex normal
		 */
		Vertex(glm::vec3 p, glm::vec3 n) {
			position = glm::vec4(p, 1);

			normal = n;
		};

		/**
		 * @param p	Vertex position
		 * @param n	Vertex normal
		 * @param tc	Texture coordinates
		 */
		Vertex(glm::vec3 p, glm::vec3 n, glm::vec3 tc) {
			position = glm::vec4(p, 1);

			normal = n;

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

			color = c;

			tex_coord = tc;
		};
	} Vertex;
}

/**
 * @class Mesh
 * @brief Manages meshes
 */
template <typename Vertex = sgltk::Vertex>
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
	void setup_shader(Shader *shader,
			  const char *model_view_matrix_name,
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

template <typename Vertex>
Mesh<Vertex>::Mesh() {
	init_lib();
	model_matrix = glm::mat4(1.0);
	shader = NULL;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
}

template <typename Vertex>
Mesh<Vertex>::~Mesh() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(ibo.size(), ibo.data());
	glDeleteVertexArrays(1, &vao);
}

template <typename Vertex>
void Mesh<Vertex>::setup_shader(Shader *shader,
				const char *model_view_matrix_name,
				const char *model_view_projection_matrix_name,
				const char *normal_matrix_name, glm::mat4 *view_matrix,
				glm::mat4 *projection_matrix) {
	this->shader = shader;
	this->model_view_matrix_name = model_view_matrix_name;
	this->model_view_projection_matrix_name =
			model_view_projection_matrix_name;
	this->normal_matrix_name = normal_matrix_name;
	this->view_matrix = view_matrix;
	this->projection_matrix = projection_matrix;
}

template <typename Vertex>
void Mesh<Vertex>::attach_vertex_array(const std::vector<Vertex> *vertexdata) {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexdata->size() * sizeof(Vertex),
		     vertexdata->data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

template <typename Vertex>
void Mesh<Vertex>::set_vertex_attribute(const char *attrib_name, GLint size,
				GLenum type, GLsizei stride,
				const GLvoid *pointer) {
	if(!shader) {
		std::cerr << "Error: No shader specified" << std::endl;
		return;
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	int loc = glGetAttribLocation(shader->shader, attrib_name);
	if(loc == -1) {
		std::cerr << "Unable to find the vertex attribute "
			  << attrib_name << std::endl;
	}
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, size, type, GL_FALSE, stride,
			      (void*)pointer);

	glBindVertexArray(0);
}

template <typename Vertex>
void Mesh<Vertex>::attach_index_array(const std::vector<unsigned short> *indices) {
	num_indices.push_back(indices->size());
	
	GLuint index;
	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		     indices->size() * sizeof(unsigned short),
		     indices->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	ibo.push_back(index);
}

template <typename Vertex>
void Mesh<Vertex>::draw(GLenum mode) {
	draw(mode, 0, NULL);
}

template <typename Vertex>
void Mesh<Vertex>::draw(GLenum mode, unsigned int index_buffer) {
	draw(mode, index_buffer, NULL);
}

template <typename Vertex>
void Mesh<Vertex>::draw(GLenum mode, glm::mat4 *model_matrix) {
	draw(mode, 0, model_matrix);
}

template <typename Vertex>
void Mesh<Vertex>::draw(GLenum mode, unsigned int index_buffer,
		glm::mat4 *model_matrix) {
	if(!shader) {
		std::cerr << "Error: No shader specified" << std::endl;
		return;
	}
	shader->bind();
	int MV_loc = glGetUniformLocation(shader->shader,
					  model_view_matrix_name);
	int NM_loc = glGetUniformLocation(shader->shader,
					  normal_matrix_name);
	int MVP_loc = glGetUniformLocation(shader->shader,
					   model_view_projection_matrix_name);
	glm::mat4 M;
	glm::mat4 MV;
	glm::mat3 NM;
	if(model_matrix)
		M = *model_matrix;
	else
		M = this->model_matrix;

	MV = (*view_matrix) * M;
	NM = glm::mat3(glm::transpose(glm::inverse(MV)));
	glm::mat4 MVP = (*projection_matrix) * MV;
	glUniformMatrix4fv(MV_loc, 1, false, glm::value_ptr(MV));
	glUniformMatrix3fv(NM_loc, 1, false, glm::value_ptr(NM));
	glUniformMatrix4fv(MVP_loc, 1, false, glm::value_ptr(MVP));

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[index_buffer]);
	glDrawElements(mode, num_indices[index_buffer],
		       GL_UNSIGNED_SHORT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shader->unbind();
}
#endif
