#include "mesh.h"

template <typename Vertex>
Mesh<Vertex>::Mesh() {
	init_lib();
	model_matrix = glm::mat4(1.0);
	shader = NULL;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	model_view_matrix_name =		"matrix.model_view";
	model_view_projection_matrix_name =	"matrix.model_view_proj";
	normal_matrix_name =			"matrix.normal_matrix";

	ambient_color_name =			"material.color_ambient";
	diffuse_color_name =			"material.color_diffuse";
	specular_color_name =			"material.color_specular";
	shininess_name =			"material.shininess";
	shininess_strength_name =		"material.shininess_strength";

	material.shininess = 0.0;
	material.shininess_strength = 1.0;
	material.color_ambient = glm::vec4(0, 0, 0, 1);
	material.color_diffuse = glm::vec4(0, 0, 0, 1);
	material.color_specular = glm::vec4(0, 0, 0, 1);
}

template <typename Vertex>
Mesh<Vertex>::~Mesh() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(ibo.size(), ibo.data());
	glDeleteVertexArrays(1, &vao);
}

template <typename Vertex>
void Mesh<Vertex>::setup_shader(Shader *shader) {
	this->shader = shader;
}

template <typename Vertex>
void Mesh<Vertex>::setup_camera(glm::mat4 *view_matrix,
				glm::mat4 *projection_matrix) {
	this->view_matrix = view_matrix;
	this->projection_matrix = projection_matrix;
}

template <typename Vertex>
void Mesh<Vertex>::setup_matrices(const char *model_view_matrix_name,
				  const char *model_view_projection_matrix_name,
				  const char *normal_matrix_name) {
	size_t len;
	if(model_view_matrix_name) {
		len = strlen(model_view_matrix_name);
		if(len > 0) {
			delete[] this->model_view_matrix_name;
			this->model_view_matrix_name = new char[len + 1];
			strncpy(this->model_view_matrix_name,
				model_view_matrix_name, len);
		}
	}

	if(model_view_projection_matrix_name) {
		len = strlen(model_view_projection_matrix_name);
		if(len > 0) {
			delete[] this->model_view_projection_matrix_name;
			this->model_view_projection_matrix_name =
				new char[len + 1];
			strncpy(this->model_view_projection_matrix_name,
				model_view_projection_matrix_name, len);
		}
	}

	if(normal_matrix_name) {
		len = strlen(normal_matrix_name);
		if(len > 0) {
			delete[] this->normal_matrix_name;
			this->normal_matrix_name = new char[len + 1];
			strncpy(this->normal_matrix_name,
				normal_matrix_name, len);
		}
	}
}

template <typename Vertex>
void Mesh<Vertex>::setup_material(const char *ambient_color_name,
				  const char *diffuse_color_name,
				  const char *specular_color_name,
				  const char *shininess_name,
				  const char *shininess_strength_name) {
	size_t len;

	if(ambient_color_name) {
		len = strlen(ambient_color_name);
		if(len > 0) {
			delete[] this->ambient_color_name;
			this->ambient_color_name = new char[len + 1];
			strncpy(this->ambient_color_name,
				ambient_color_name, len);
		}
	}

	if(diffuse_color_name) {
		len = strlen(diffuse_color_name);
		if(len > 0) {
			delete[] this->diffuse_color_name;
			this->diffuse_color_name = new char[len + 1];
			strncpy(this->diffuse_color_name,
				diffuse_color_name, len);
		}
	}

	if(specular_color_name) {
		len = strlen(specular_color_name);
		if(len > 0) {
			delete[] this->specular_color_name;
			this->specular_color_name = new char[len + 1];
			strncpy(this->specular_color_name,
				specular_color_name, len);
		}
	}

	if(shininess_name) {
		len = strlen(shininess_name);
		if(len > 0) {
			delete[] this->shininess_name;
			this->shininess_name = new char[len + 1];
			strncpy(this->shininess_name,
				shininess_name, len);
		}
	}

	if(shininess_strength_name) {
		len = strlen(shininess_strength_name);
		if(len > 0) {
			delete[] this->shininess_strength_name;
			this->shininess_strength_name = new char[len + 1];
			strncpy(this->shininess_strength_name,
				shininess_strength_name, len);
		}
	}
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
void Mesh<Vertex>::compute_bounding_box(const std::vector<Vertex> *vertexdata) {
}

template <typename Vertex>
int Mesh<Vertex>::set_vertex_attribute(const char *attrib_name, GLint size,
				GLenum type, GLsizei stride,
				const GLvoid *pointer) {
	if(!shader) {
		return -1;
	}
	glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	int loc = glGetAttribLocation(shader->shader, attrib_name);
	if(loc == -1) {
		return -2;
	}
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, size, type, GL_FALSE, stride,
			      (void*)pointer);

	glBindVertexArray(0);
	return 0;
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

	shader->bind();
	int loc = glGetUniformLocation(shader->shader,
					  model_view_matrix_name);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(MV));

	loc = glGetUniformLocation(shader->shader,
					  normal_matrix_name);
	glUniformMatrix3fv(loc, 1, false, glm::value_ptr(NM));

	loc = glGetUniformLocation(shader->shader,
					   model_view_projection_matrix_name);
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(MVP));

	loc = glGetUniformLocation(shader->shader, ambient_color_name);
	glUniform4f(loc, material.color_ambient.x, material.color_ambient.y,
		    material.color_ambient.z, material.color_ambient.w);

	loc = glGetUniformLocation(shader->shader, diffuse_color_name);
	glUniform4f(loc, material.color_diffuse.x, material.color_diffuse.y,
		    material.color_diffuse.z, material.color_diffuse.w);

	loc = glGetUniformLocation(shader->shader, specular_color_name);
	glUniform4f(loc, material.color_specular.x, material.color_specular.y,
		    material.color_specular.z, material.color_specular.w);

	loc = glGetUniformLocation(shader->shader, shininess_name);
	glUniform1f(loc, material.shininess);

	loc = glGetUniformLocation(shader->shader, shininess_strength_name);
	glUniform1f(loc, material.shininess_strength);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[index_buffer]);
	glDrawElements(mode, num_indices[index_buffer],
		       GL_UNSIGNED_SHORT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shader->unbind();
}
