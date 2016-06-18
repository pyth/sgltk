#include "mesh.h"

using namespace sgltk;

Mesh::Mesh() {
	sgltk::init_lib();
	model_matrix = glm::mat4(1.0);
	shader = NULL;
	glGenVertexArrays(1, &vao);

	bounding_box.push_back(glm::vec3(0, 0, 0));
	bounding_box.push_back(glm::vec3(0, 0, 0));

	model_matrix_name =			"matrix.model_matrix";
	view_matrix_name =			"matrix.view_matrix";
	projection_matrix_name =		"matrix.proj_matrix";
	model_view_matrix_name =		"matrix.model_view";
	view_proj_matrix_name =			"matrix.view_proj";
	model_view_projection_matrix_name =	"matrix.model_view_proj";
	normal_matrix_name =			"matrix.normal_matrix";

	ambient_color_name =			"color_ambient";
	diffuse_color_name =			"color_diffuse";
	specular_color_name =			"color_specular";
	shininess_name =			"shininess";
	shininess_strength_name =		"shininess_strength";

	ambient_texture_name =			"texture_ambient";
	diffuse_texture_name =			"texture_diffuse";
	specular_texture_name =			"texture_specular";
	shininess_texture_name =		"texture_shininess";
	emmisive_texture_name =			"texture_emmisive";
	normals_texture_name =			"texture_normals";
	displacement_texture_name =		"texture_displacement";
	opacity_texture_name =			"texture_opacity";
	lightmap_texture_name =			"texture_lightmap";

	shininess = 0.0;
	shininess_strength = 1.0;
	color_ambient = glm::vec4(0, 0, 0, 1);
	color_diffuse = glm::vec4(0, 0, 0, 1);
	color_specular = glm::vec4(0, 0, 0, 1);
}

Mesh::~Mesh() {
	glDeleteBuffers(vbo.size(), vbo.data());
	glDeleteBuffers(ibo.size(), ibo.data());
	glDeleteVertexArrays(1, &vao);
}

void Mesh::setup_shader(Shader *shader) {
	this->shader = shader;
}

void Mesh::setup_camera(glm::mat4 *view_matrix,
				glm::mat4 *projection_matrix) {
	this->view_matrix = view_matrix;
	this->projection_matrix = projection_matrix;
}

void Mesh::set_model_matrix_name(std::string name) {

	if(name.length() > 0)
		model_matrix_name = name;
	else
		model_matrix_name = "matrix.model_matrix";
}

void Mesh::set_view_matrix_name(std::string name) {

	if(name.length() > 0)
		view_matrix_name = name;
	else
		view_matrix_name = "matrix.view_matrix";
}

void Mesh::set_projection_matrix_name(std::string name) {

	if(name.length() > 0)
		projection_matrix_name = name;
	else
		projection_matrix_name = "matrix.proj_matrix";
}

void Mesh::set_model_view_matrix_name(std::string name) {

	if(name.length() > 0)
		model_view_matrix_name = name;
	else
		model_view_matrix_name = "matrix.model_view";
}

void Mesh::set_view_proj_matrix_name(std::string name) {

	if(name.length() > 0)
		view_proj_matrix_name = name;
	else
		view_proj_matrix_name = "matrix.view_proj";
}

void Mesh::set_model_view_proj_name(std::string name) {

	if(name.length() > 0)
		model_view_projection_matrix_name = name;
	else
		model_view_projection_matrix_name = "matrix.model_view_proj";
}

void Mesh::set_normal_matrix_name(std::string name) {

	if(name.length() > 0)
		normal_matrix_name = name;
	else
		normal_matrix_name = "matrix.normal_matrix";
}

void Mesh::set_ambient_color_name(std::string name) {
	if(name.length() > 0)
		ambient_color_name = name;
	else
		ambient_color_name = "color_ambient";
}

void Mesh::set_diffuse_color_name(std::string name) {
	if(name.length() > 0)
		diffuse_color_name = name;
	else
		diffuse_color_name = "color_diffuse";
}

void Mesh::set_specular_color_name(std::string name) {
	if(name.length() > 0)
		specular_color_name = name;
	else
		specular_color_name = "color_specular";
}

void Mesh::set_shininess_name(std::string name) {
	if(name.length() > 0)
		shininess_name = name;
	else
		shininess_name = "shininess_name";
}

void Mesh::set_shininess_strength_name(std::string name) {
	if(name.length() > 0)
		shininess_strength_name = name;
	else
		shininess_strength_name = "shininess_strength";
}

void Mesh::set_ambient_texture_name(std::string name) {
	if(name.length() > 0)
		ambient_texture_name = name;
	else
		ambient_texture_name = "texture_ambient";
}

void Mesh::set_diffuse_texture_name(std::string name) {
	if(name.length() > 0)
		diffuse_texture_name = name;
	else
		diffuse_texture_name = "texture_diffuse";
}

void Mesh::set_specular_texture_name(std::string name) {
	if(name.length() > 0)
		specular_texture_name = name;
	else
		specular_texture_name = "texture_specular";
}

void Mesh::set_shininess_texture_name(std::string name) {
	if(name.length() > 0)
		shininess_texture_name = name;
	else
		shininess_texture_name = "texture_shininess";
}

void Mesh::set_emmisive_texture_name(std::string name) {
	if(name.length() > 0)
		emmisive_texture_name = name;
	else
		emmisive_texture_name = "texture_emmisive";
}

void Mesh::set_normals_texture_name(std::string name) {
	if(name.length() > 0)
		normals_texture_name = name;
	else
		normals_texture_name = "texture_normals";
}

void Mesh::set_displacement_texture_name(std::string name) {
	if(name.length() > 0)
		displacement_texture_name = name;
	else
		displacement_texture_name = "texture_displacement";
}

void Mesh::set_opacity_texture_name(std::string name) {
	if(name.length() > 0)
		opacity_texture_name = name;
	else
		opacity_texture_name = "texture_opacity";
}

void Mesh::set_lightmap_texture_name(std::string name) {
	if(name.length() > 0)
		lightmap_texture_name = name;
	else
		lightmap_texture_name = "texture_lightmap";
}

int Mesh::set_vertex_attribute(std::string attrib_name,
				unsigned int buffer_index,
				GLint number_elements,
				GLenum type,
				GLsizei stride,
				const GLvoid *pointer) {
	int loc = glGetAttribLocation(shader->program, attrib_name.c_str());
	if(loc == -1) {
		return -2;
	}

	return set_vertex_attribute(loc, buffer_index, number_elements, type,
					stride, pointer);
}

int Mesh::set_vertex_attribute(int attrib_location,
				unsigned int buffer_index,
				GLint number_elements,
				GLenum type,
				GLsizei stride,
				const GLvoid *pointer) {
	if(!shader) {
		return -1;
	}

	if(attrib_location == -1) {
		return -2;
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[buffer_index]);

	glEnableVertexAttribArray(attrib_location);
	switch(type) {
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
		case GL_INT:
		case GL_UNSIGNED_INT:
			glVertexAttribIPointer(attrib_location, number_elements,
						type, stride, (void *)pointer);
			break;
		case GL_DOUBLE:
			glVertexAttribLPointer(attrib_location, number_elements,
						type, stride, (void *)pointer);
			break;
		default:
			glVertexAttribPointer(attrib_location, number_elements,
						type, GL_FALSE, stride, 
						(void*)pointer);
			break;
	}

	glBindVertexArray(0);
	return 0;
}

void Mesh::set_vertex_attribute_divisor(std::string attrib_name,
						unsigned int divisor) {
	int loc = glGetAttribLocation(shader->program, attrib_name.c_str());
	if(loc == -1) {
		return;
	}

	glVertexAttribDivisor(loc, divisor);
}

void Mesh::set_vertex_attribute_divisor(unsigned int attrib_location,
					unsigned int divisor) {
	if(attrib_location < 0)
		return;

	glVertexAttribDivisor(attrib_location, divisor);
}

void Mesh::attach_index_buffer(const std::vector<unsigned short> *indices) {
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

void Mesh::material_uniform() {
	int loc;

	loc = glGetUniformLocation(shader->program, ambient_color_name.c_str());
	glUniform4f(loc, color_ambient.x, color_ambient.y, color_ambient.z,
		    color_ambient.w);

	loc = glGetUniformLocation(shader->program, diffuse_color_name.c_str());
	glUniform4f(loc, color_diffuse.x, color_diffuse.y, color_diffuse.z,
		    color_diffuse.w);

	loc = glGetUniformLocation(shader->program, specular_color_name.c_str());
	glUniform4f(loc, color_specular.x, color_specular.y, color_specular.z,
		    color_specular.w);

	loc = glGetUniformLocation(shader->program, shininess_name.c_str());
	glUniform1f(loc, shininess);

	loc = glGetUniformLocation(shader->program, shininess_strength_name.c_str());
	glUniform1f(loc, shininess_strength);

	unsigned int num_textures = 0;
	for(unsigned int i = 0; i < textures_ambient.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				ambient_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = ambient_texture_name +
				'[' + std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_ambient[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_diffuse.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				diffuse_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = diffuse_texture_name +
				'[' + std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_diffuse[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_specular.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				specular_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = specular_texture_name +
				'[' + std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_specular[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_shininess.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				shininess_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = shininess_texture_name +
				'[' + std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_shininess[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_emmisive.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				emmisive_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = emmisive_texture_name +
				'[' + std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_emmisive[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_normals.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				normals_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = normals_texture_name +
				'[' + std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_normals[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_displacement.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				displacement_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = displacement_texture_name +
				'[' + std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_displacement[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_opacity.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				opacity_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = opacity_texture_name +
				'[' + std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_opacity[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_lightmap.size(); i++) {
		int texture_loc = glGetUniformLocation(shader->program,
				lightmap_texture_name.c_str());
		if(texture_loc < 0) {
			std::string uniform_name = lightmap_texture_name +
				'[' + std::to_string(i) + ']';
			texture_loc = glGetUniformLocation(shader->program,
					uniform_name.c_str());
		}
		glUniform1i(texture_loc, num_textures);
		textures_lightmap[i]->bind(num_textures);
		num_textures++;
	}

}

void Mesh::draw(GLenum mode) {
	draw(mode, 0, NULL);
}

void Mesh::draw(GLenum mode, unsigned int index_buffer) {
	draw(mode, index_buffer, NULL);
}

void Mesh::draw(GLenum mode, glm::mat4 *model_matrix) {
	draw(mode, 0, model_matrix);
}

void Mesh::draw(GLenum mode, unsigned int index_buffer,
			glm::mat4 *model_matrix) {

	if(!shader) {
		error_string = "Error: No shader specified";
		return;
	}

	int loc;
	glm::mat4 M;
	glm::mat4 MV;
	glm::mat4 MVP;
	glm::mat4 VP;
	glm::mat3 NM;
	if(model_matrix)
		M = *model_matrix;
	else
		M = this->model_matrix;

	NM = glm::mat3(glm::transpose(glm::inverse(M)));
	MV = (*view_matrix) * M;
	MVP = (*projection_matrix) * MV;
	VP = (*projection_matrix) * (*view_matrix);

	shader->bind();
	loc = glGetUniformLocation(shader->program,
				       model_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(M));

	loc = glGetUniformLocation(shader->program,
				       view_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(*view_matrix));

	loc = glGetUniformLocation(shader->program,
				   projection_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(*projection_matrix));

	loc = glGetUniformLocation(shader->program,
				       model_view_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(MV));

	loc = glGetUniformLocation(shader->program,
				       view_proj_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(VP));

	loc = glGetUniformLocation(shader->program,
				   normal_matrix_name.c_str());
	glUniformMatrix3fv(loc, 1, false, glm::value_ptr(NM));

	loc = glGetUniformLocation(shader->program,
				   model_view_projection_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(MVP));

	material_uniform();

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[index_buffer]);
	glDrawElements(mode, num_indices[index_buffer],
		       GL_UNSIGNED_SHORT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shader->unbind();
}

void Mesh::draw_instanced(GLenum mode, std::vector<glm::mat4> *model_matrices) {
	draw_instanced(mode, 0, model_matrices);
}

void Mesh::draw_instanced(GLenum mode, unsigned int index_buffer,
			std::vector<glm::mat4> *model_matrices) {

	if(!shader) {
		error_string = "Error: No shader specified";
		return;
	}

	if(model_matrices == NULL) {
		draw(mode, index_buffer);
		return;
	}

	if(model_matrices->size() == 0) {
		draw(mode, index_buffer);
		return;
	}

	if(model_matrices->size() == 1) {
		draw(mode, index_buffer, &(*model_matrices)[0]);
		return;
	}

	int loc;
	glm::mat4 M;
	glm::mat4 MV;
	glm::mat4 MVP;
	glm::mat4 VP;
	glm::mat3 NM;

	VP = (*projection_matrix) * (*view_matrix);
	shader->bind();

	for(unsigned int i = 0; i < model_matrices->size(); i++) {
		std::string index = "[" + std::to_string(i) + "]";
		M = (*model_matrices)[i];
		NM = glm::mat3(glm::transpose(glm::inverse(M)));
		MV = (*view_matrix) * M;
		MVP = (*projection_matrix) * MV;

		loc = glGetUniformLocation(shader->program,
				(model_matrix_name + index).c_str());
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(M));

		loc = glGetUniformLocation(shader->program,
				(model_view_matrix_name + index).c_str());
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(MV));

		loc = glGetUniformLocation(shader->program,
				(normal_matrix_name + index).c_str());
		glUniformMatrix3fv(loc, 1, false, glm::value_ptr(NM));

		loc = glGetUniformLocation(shader->program,
				(model_view_projection_matrix_name + index).c_str());
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(MVP));
	}

	loc = glGetUniformLocation(shader->program,
			view_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(*view_matrix));

	loc = glGetUniformLocation(shader->program,
			projection_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(*projection_matrix));

	loc = glGetUniformLocation(shader->program,
			view_proj_matrix_name.c_str());
	glUniformMatrix4fv(loc, 1, false, glm::value_ptr(VP));

	material_uniform();

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[index_buffer]);
	glDrawElementsInstanced(mode, num_indices[index_buffer],
		       GL_UNSIGNED_SHORT, (void*)0, model_matrices->size());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shader->unbind();
}
