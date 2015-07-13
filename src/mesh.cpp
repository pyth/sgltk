#include "mesh.h"

using namespace std;

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
void Mesh<Vertex>::setup_shader(Shader *shader, const char *model_view_matrix_name,
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
	glm::mat4 MV;
	if(model_matrix)
		MV = (*view_matrix) * (*model_matrix);
	else
		MV = (*view_matrix) * this->model_matrix;
	glm::mat3 NM = glm::transpose(glm::inverse(glm::mat3(MV)));
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

template class Mesh<sgltk::Vertex>;
