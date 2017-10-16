#include "mesh.h"

using namespace sgltk;

Mesh::Mesh() {
	tf_mode = GL_NONE;
	model_matrix = glm::mat4(1.0);
	shader = nullptr;
	num_uv = 0;
	num_col = 0;
	num_vertices = 0;
	index_type = 0;
	glGenVertexArrays(1, &vao);

	view_matrix = NULL;
	projection_matrix = NULL;

	bounding_box = { glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) };

	model_matrix_name =			"model_matrix";
	view_matrix_name =			"view_matrix";
	projection_matrix_name =		"proj_matrix";
	model_view_matrix_name =		"model_view_matrix";
	view_proj_matrix_name =			"view_proj_matrix";
	model_view_projection_matrix_name =	"model_view_proj_matrix";
	normal_matrix_name =			"normal_matrix";

	ambient_color_name =			"color_ambient";
	diffuse_color_name =			"color_diffuse";
	specular_color_name =			"color_specular";
	shininess_name =			"shininess";
	shininess_strength_name =		"shininess_strength";

	ambient_texture_name =			"texture_ambient";
	diffuse_texture_name =			"texture_diffuse";
	specular_texture_name =			"texture_specular";
	shininess_texture_name =		"texture_shininess";
	emissive_texture_name =			"texture_emissive";
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
	glDeleteVertexArrays(1, &vao);
}

void Mesh::setup_shader(Shader *shader) {
	this->shader = shader;
}

bool Mesh::setup_camera(glm::mat4 *view_matrix,
				glm::mat4 *projection_matrix) {
	if(!view_matrix || !projection_matrix)
		return false;

	this->view_matrix = view_matrix;
	this->projection_matrix = projection_matrix;
	return true;
}

bool Mesh::setup_camera(Camera *camera) {
	if(!camera)
		return false;

	this->view_matrix = &camera->view_matrix;
	this->projection_matrix = &camera->projection_matrix;
	return true;
}

void Mesh::set_model_matrix_name(const std::string& name) {

	if(name.length() > 0)
		model_matrix_name = name;
	else
		model_matrix_name = "model_matrix";
}

void Mesh::set_view_matrix_name(const std::string& name) {

	if(name.length() > 0)
		view_matrix_name = name;
	else
		view_matrix_name = "view_matrix";
}

void Mesh::set_projection_matrix_name(const std::string& name) {

	if(name.length() > 0)
		projection_matrix_name = name;
	else
		projection_matrix_name = "proj_matrix";
}

void Mesh::set_model_view_matrix_name(const std::string& name) {

	if(name.length() > 0)
		model_view_matrix_name = name;
	else
		model_view_matrix_name = "model_view_matrix";
}

void Mesh::set_view_proj_matrix_name(const std::string& name) {

	if(name.length() > 0)
		view_proj_matrix_name = name;
	else
		view_proj_matrix_name = "view_proj_matrix";
}

void Mesh::set_model_view_proj_name(const std::string& name) {

	if(name.length() > 0)
		model_view_projection_matrix_name = name;
	else
		model_view_projection_matrix_name = "model_view_proj_matrix";
}

void Mesh::set_normal_matrix_name(const std::string& name) {

	if(name.length() > 0)
		normal_matrix_name = name;
	else
		normal_matrix_name = "normal_matrix";
}

void Mesh::set_ambient_color_name(const std::string& name) {
	if(name.length() > 0)
		ambient_color_name = name;
	else
		ambient_color_name = "color_ambient";
}

void Mesh::set_diffuse_color_name(const std::string& name) {
	if(name.length() > 0)
		diffuse_color_name = name;
	else
		diffuse_color_name = "color_diffuse";
}

void Mesh::set_specular_color_name(const std::string& name) {
	if(name.length() > 0)
		specular_color_name = name;
	else
		specular_color_name = "color_specular";
}

void Mesh::set_shininess_name(const std::string& name) {
	if(name.length() > 0)
		shininess_name = name;
	else
		shininess_name = "shininess_name";
}

void Mesh::set_shininess_strength_name(const std::string& name) {
	if(name.length() > 0)
		shininess_strength_name = name;
	else
		shininess_strength_name = "shininess_strength";
}

void Mesh::set_ambient_texture_name(const std::string& name) {
	if(name.length() > 0)
		ambient_texture_name = name;
	else
		ambient_texture_name = "texture_ambient";
}

void Mesh::set_diffuse_texture_name(const std::string& name) {
	if(name.length() > 0)
		diffuse_texture_name = name;
	else
		diffuse_texture_name = "texture_diffuse";
}

void Mesh::set_specular_texture_name(const std::string& name) {
	if(name.length() > 0)
		specular_texture_name = name;
	else
		specular_texture_name = "texture_specular";
}

void Mesh::set_shininess_texture_name(const std::string& name) {
	if(name.length() > 0)
		shininess_texture_name = name;
	else
		shininess_texture_name = "texture_shininess";
}

void Mesh::set_emissive_texture_name(const std::string& name) {
	if(name.length() > 0)
		emissive_texture_name = name;
	else
		emissive_texture_name = "texture_emissive";
}

void Mesh::set_normals_texture_name(const std::string& name) {
	if(name.length() > 0)
		normals_texture_name = name;
	else
		normals_texture_name = "texture_normals";
}

void Mesh::set_displacement_texture_name(const std::string& name) {
	if(name.length() > 0)
		displacement_texture_name = name;
	else
		displacement_texture_name = "texture_displacement";
}

void Mesh::set_opacity_texture_name(const std::string& name) {
	if(name.length() > 0)
		opacity_texture_name = name;
	else
		opacity_texture_name = "texture_opacity";
}

void Mesh::set_lightmap_texture_name(const std::string& name) {
	if(name.length() > 0)
		lightmap_texture_name = name;
	else
		lightmap_texture_name = "texture_lightmap";
}

void Mesh::set_transform_feedback_mode(GLenum mode) {
	tf_mode = mode;
}

void Mesh::attach_buffer(sgltk::Buffer *buffer,
			 GLuint target,
			 unsigned int index) {

	attached_buffers.push_back(buffer);
	attached_buffers_targets.push_back(target);
	attached_buffers_indices.push_back(index);
}

int Mesh::set_vertex_attribute(const std::string& attrib_name,
				unsigned int buffer_index,
				GLint number_elements,
				GLenum type,
				GLsizei stride,
				const GLvoid *pointer,
				unsigned int divisor) {

	if(!shader) {
		return -1;
	}

	int loc = shader->get_attribute_location(attrib_name);
	if(loc < 0) {
		return -2;
	}

	return set_vertex_attribute(loc, buffer_index, number_elements, type,
					stride, pointer, divisor);
}

int Mesh::set_vertex_attribute(int attrib_location,
				unsigned int buffer_index,
				GLint number_elements,
				GLenum type,
				GLsizei stride,
				const GLvoid *pointer,
				unsigned int divisor) {

	if(attrib_location < 0) {
		return -2;
	}

	glBindVertexArray(vao);
	vbo[buffer_index]->bind();

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

	glVertexAttribDivisor(attrib_location, divisor);
	glBindVertexArray(0);
	return 0;
}

int Mesh::set_buffer_vertex_attribute(const std::string& attrib_name,
				sgltk::Buffer *buffer,
				GLint number_elements,
				GLenum type,
				GLsizei stride,
				const GLvoid *pointer,
				unsigned int divisor) {

	if(!shader) {
		return -1;
	}

	int loc = shader->get_attribute_location(attrib_name);
	if(loc < 0) {
		return -2;
	}

	return set_buffer_vertex_attribute(loc, buffer, number_elements, type,
							stride, pointer, divisor);
}

int Mesh::set_buffer_vertex_attribute(int attrib_location,
				sgltk::Buffer *buffer,
				GLint number_elements,
				GLenum type,
				GLsizei stride,
				const GLvoid *pointer,
				unsigned int divisor) {

	if(attrib_location < 0) {
		return -2;
	}

	glBindVertexArray(vao);
	buffer->bind();

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

	glVertexAttribDivisor(attrib_location, divisor);
	glBindVertexArray(0);
	return 0;
}

int Mesh::attach_index_buffer(const std::vector<unsigned char>& indices) {
	if(index_type && index_type != GL_UNSIGNED_BYTE)
		return -1;

	index_type = GL_UNSIGNED_BYTE;
	std::unique_ptr<Buffer> index(new Buffer(GL_ELEMENT_ARRAY_BUFFER));
	index->load<unsigned char>(indices, GL_STATIC_DRAW);
	ibo.push_back(std::move(index));
	return ibo.size() - 1;
}

int Mesh::attach_index_buffer(const std::vector<unsigned short>& indices) {
	if(index_type && index_type != GL_UNSIGNED_SHORT)
		return -1;

	index_type = GL_UNSIGNED_SHORT;
	std::unique_ptr<Buffer> index(new Buffer(GL_ELEMENT_ARRAY_BUFFER));
	index->load<unsigned short>(indices, GL_STATIC_DRAW);
	ibo.push_back(std::move(index));
	return ibo.size() - 1;
}

int Mesh::attach_index_buffer(const std::vector<unsigned int>& indices) {
	if(index_type && index_type != GL_UNSIGNED_INT)
		return -1;

	index_type = GL_UNSIGNED_INT;
	std::unique_ptr<Buffer> index(new Buffer(GL_ELEMENT_ARRAY_BUFFER));
	index->load<unsigned int>(indices, GL_STATIC_DRAW);
	ibo.push_back(std::move(index));
	return ibo.size() - 1;
}

void Mesh::material_uniform() {
	shader->set_uniform(ambient_color_name, color_ambient);
	shader->set_uniform(diffuse_color_name, color_diffuse);
	shader->set_uniform(specular_color_name, color_specular);
	shader->set_uniform_float(shininess_name, shininess);
	shader->set_uniform_float(shininess_strength_name, shininess_strength);

	int num_textures = 0;
	int texture_loc = shader->get_uniform_location(ambient_texture_name);
	for(unsigned int i = 0; i < textures_ambient.size(); i++) {
		shader->set_uniform_int(texture_loc + i, num_textures);
		textures_ambient[i]->bind(num_textures);
		num_textures++;
	}

	texture_loc = shader->get_uniform_location(diffuse_texture_name);
	for(unsigned int i = 0; i < textures_diffuse.size(); i++) {
		shader->set_uniform_int(texture_loc + i, num_textures);
		textures_diffuse[i]->bind(num_textures);
		num_textures++;
	}

	texture_loc = shader->get_uniform_location(specular_texture_name);
	for(unsigned int i = 0; i < textures_specular.size(); i++) {
		shader->set_uniform_int(texture_loc + i, num_textures);
		textures_specular[i]->bind(num_textures);
		num_textures++;
	}

	texture_loc = shader->get_uniform_location(shininess_texture_name);
	for(unsigned int i = 0; i < textures_shininess.size(); i++) {
		shader->set_uniform_int(texture_loc + i, num_textures);
		textures_shininess[i]->bind(num_textures);
		num_textures++;
	}

	texture_loc = shader->get_uniform_location(emissive_texture_name);
	for(unsigned int i = 0; i < textures_emissive.size(); i++) {
		shader->set_uniform_int(texture_loc + i, num_textures);
		textures_emissive[i]->bind(num_textures);
		num_textures++;
	}

	texture_loc = shader->get_uniform_location(normals_texture_name);
	for(unsigned int i = 0; i < textures_normals.size(); i++) {
		shader->set_uniform_int(texture_loc + i, num_textures);
		textures_normals[i]->bind(num_textures);
		num_textures++;
	}

	texture_loc = shader->get_uniform_location(displacement_texture_name);
	for(unsigned int i = 0; i < textures_displacement.size(); i++) {
		shader->set_uniform_int(texture_loc + i, num_textures);
		textures_displacement[i]->bind(num_textures);
		num_textures++;
	}

	texture_loc = shader->get_uniform_location(opacity_texture_name);
	for(unsigned int i = 0; i < textures_opacity.size(); i++) {
		shader->set_uniform_int(texture_loc + i, num_textures);
		textures_opacity[i]->bind(num_textures);
		num_textures++;
	}

	texture_loc = shader->get_uniform_location(lightmap_texture_name);
	for(unsigned int i = 0; i < textures_lightmap.size(); i++) {
		shader->set_uniform_int(texture_loc + i, num_textures);
		textures_lightmap[i]->bind(num_textures);
		num_textures++;
	}

	for(unsigned int i = 0; i < textures_misc.size(); i++) {
		texture_loc = shader->get_uniform_location(textures_misc[i].first);
		shader->set_uniform_int(texture_loc, num_textures);
		textures_misc[i].second->bind(num_textures);
		num_textures++;
	}
}

void Mesh::draw(GLenum mode) {
	draw(mode, 0, NULL);
}

void Mesh::draw(GLenum mode, const glm::mat4 *model_matrix) {
	draw(mode, 0, model_matrix);
}

void Mesh::draw(GLenum mode,
		unsigned int index_buffer,
		const glm::mat4 *model_matrix = NULL) {

	if(!shader) {
		App::error_string.push_back("Error: No shader specified");
		return;
	}

	glm::mat4 M;
	glm::mat4 MV;
	glm::mat4 MVP;
	glm::mat4 VP;
	glm::mat3 NM;
	if(model_matrix)
		M = *model_matrix;
	else
		M = this->model_matrix;
	shader->set_uniform(model_matrix_name, false, M);

	NM = glm::transpose(glm::inverse(glm::mat3(M)));
	shader->set_uniform(normal_matrix_name, false, NM);

	if(view_matrix) {
		MV = (*view_matrix) * M;
		shader->set_uniform(view_matrix_name, false, *view_matrix);
		shader->set_uniform(model_view_matrix_name, false, MV);
	}
	if(projection_matrix) {
		MVP = (*projection_matrix) * MV;
		shader->set_uniform(projection_matrix_name, false,
						*projection_matrix);
	}
	if(view_matrix && projection_matrix) {
		VP = (*projection_matrix) * (*view_matrix);
		shader->set_uniform(view_proj_matrix_name, false, VP);
		shader->set_uniform(model_view_projection_matrix_name,
								false, MVP);
	}

	material_uniform();

	for(unsigned int i = 0; i < attached_buffers.size(); i++) {
		attached_buffers[i]->bind(attached_buffers_targets[i],
					  attached_buffers_indices[i]);
	}

	glBindVertexArray(vao);
	ibo[index_buffer]->bind();
	if(shader->transform_feedback) {
		GLenum primitive_type = tf_mode;
		if(primitive_type == GL_NONE) {
			switch(mode) {
				case GL_POINTS:
					primitive_type = GL_POINTS;
					break;
				case GL_LINES:
				case GL_LINE_LOOP:
				case GL_LINE_STRIP:
				case GL_LINES_ADJACENCY:
				case GL_LINE_STRIP_ADJACENCY:
					primitive_type = GL_LINES;
					break;
				case GL_TRIANGLES:
				case GL_TRIANGLE_STRIP:
				case GL_TRIANGLE_FAN:
				case GL_TRIANGLES_ADJACENCY:
				case GL_TRIANGLE_STRIP_ADJACENCY:
					primitive_type = GL_TRIANGLES;
					break;
				default:
					break;
			}
		}
		glBeginTransformFeedback(primitive_type);
	}
	glDrawElements(mode, ibo[index_buffer]->num_elements,
		       index_type, (void*)0);
	if(shader->transform_feedback) {
		glEndTransformFeedback();
	}
	ibo[index_buffer]->unbind();
	glBindVertexArray(0);

	for(unsigned int i = 0; i < attached_buffers.size(); i++) {
		attached_buffers[i]->unbind();
	}
}

void Mesh::draw_instanced(GLenum mode, unsigned int num_instances) {
	draw_instanced(mode, 0, num_instances);
}

void Mesh::draw_instanced(GLenum mode, unsigned int index_buffer,
					unsigned int num_instances) {

	if(!shader) {
		App::error_string.push_back("Error: No shader specified");
		return;
	}

	glm::mat4 VP = (*projection_matrix) * (*view_matrix);

	shader->set_uniform(view_matrix_name, false, *view_matrix);
	shader->set_uniform(projection_matrix_name, false, *projection_matrix);
	shader->set_uniform(view_proj_matrix_name, false, VP);

	material_uniform();

	for(unsigned int i = 0; i < attached_buffers.size(); i++) {
		attached_buffers[i]->bind(attached_buffers_targets[i],
					  attached_buffers_indices[i]);
	}

	glBindVertexArray(vao);
	ibo[index_buffer]->bind();
	if(shader->transform_feedback) {
		GLenum primitive_type = tf_mode;
		if(primitive_type == GL_NONE) {
			switch(mode) {
				case GL_POINTS:
					primitive_type = GL_POINTS;
					break;
				case GL_LINES:
				case GL_LINE_LOOP:
				case GL_LINE_STRIP:
				case GL_LINES_ADJACENCY:
				case GL_LINE_STRIP_ADJACENCY:
					primitive_type = GL_LINES;
					break;
				case GL_TRIANGLES:
				case GL_TRIANGLE_STRIP:
				case GL_TRIANGLE_FAN:
				case GL_TRIANGLES_ADJACENCY:
				case GL_TRIANGLE_STRIP_ADJACENCY:
					primitive_type = GL_TRIANGLES;
					break;
				default:
					break;
			}
		}
		glBeginTransformFeedback(primitive_type);
	}
	glDrawElementsInstanced(mode, ibo[index_buffer]->num_elements,
		       index_type, (void*)0, num_instances);
	if(shader->transform_feedback) {
		glEndTransformFeedback();
	}
	ibo[index_buffer]->unbind();
	glBindVertexArray(0);

	for(unsigned int i = 0; i < attached_buffers.size(); i++) {
		attached_buffers[i]->unbind();
	}
}
