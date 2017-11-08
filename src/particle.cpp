#include "particle.h"

using namespace sgltk;

Particles::Particles() {
	timer.start();
}

Particles::~Particles() {
}

void Particles::resize(unsigned int number) {
	this->number = number;
	position.resize(number);
	velocity.resize(number);
	lifetime.resize(number);
	indices.resize(number);
	for(unsigned int i = 0; i < number; i++) {
		indices[i] = i;
	}
	pos_buf = mesh.attach_vertex_buffer<glm::vec3>(position, GL_STREAM_DRAW);
	vel_buf = mesh.attach_vertex_buffer<glm::vec3>(velocity, GL_STREAM_DRAW);
	life_buf = mesh.attach_vertex_buffer<glm::vec2>(lifetime, GL_STREAM_DRAW);
	mesh.attach_index_buffer(indices);
	mesh.set_vertex_attribute("position_in", pos_buf, 3, GL_FLOAT, 0, 0);
	mesh.set_vertex_attribute("velocity_in", vel_buf, 3, GL_FLOAT, 0, 0);
	mesh.set_vertex_attribute("lifetime_in", life_buf, 2, GL_FLOAT, 0, 0);
}

void Particles::setup_shader(Shader *shader) {
	mesh.setup_shader(shader);
}

void Particles::setup_camera(Camera *camera) {
	mesh.setup_camera(camera);
}

void Particles::setup_camera(glm::mat4 *view_matrix,
				glm::mat4 *projection_matrix) {

	mesh.setup_camera(view_matrix, projection_matrix);
}

void Particles::attach_texture(const std::string& name,
			       const Texture& texture) {

	mesh.attach_texture(name, texture);
}

bool Particles::add_particle(glm::vec3 position,
				glm::vec3 velocity,
				float lifetime) {

	float time = (float)timer.get_time_s();
	for(unsigned int i = 0; i < number; i++) {
		if(this->lifetime[i][0] + this->lifetime[i][1] < time) {
			this->position[i] = position;
			this->velocity[i] = velocity;
			this->lifetime[i] = glm::vec2(time, lifetime);
			break;
		}
	}
	return true;
}

bool Particles::add_particle_immediately(glm::vec3 position,
					glm::vec3 velocity,
					float lifetime) {

	float time = (float)timer.get_time_s();
	for(unsigned int i = 0; i < number; i++) {
		if(this->lifetime[i][0] + this->lifetime[i][1] < time) {
			this->position[i] = position;
			this->velocity[i] = velocity;
			this->lifetime[i] = glm::vec2(time, lifetime);
			mesh.replace_partial_data<glm::vec3>(pos_buf, i * sizeof(glm::vec3), &this->position[i], 1);
			mesh.replace_partial_data<glm::vec3>(vel_buf, i * sizeof(glm::vec3), &this->velocity[i], 1);
			mesh.replace_partial_data<glm::vec2>(life_buf, i * sizeof(glm::vec2), &this->lifetime[i], 1);
			mesh.set_vertex_attribute("position_in", pos_buf, 3, GL_FLOAT, 0, 0);
			mesh.set_vertex_attribute("velocity_in", vel_buf, 3, GL_FLOAT, 0, 0);
			mesh.set_vertex_attribute("lifetime_in", life_buf, 2, GL_FLOAT, 0, 0);
			break;
		}
	}
	return true;
}

void Particles::update_all() {
	mesh.replace_buffer_data<glm::vec3>(pos_buf, position);
	mesh.replace_buffer_data<glm::vec3>(vel_buf, velocity);
	mesh.replace_buffer_data<glm::vec2>(life_buf, lifetime);
	mesh.set_vertex_attribute("position_in", pos_buf, 3, GL_FLOAT, 0, 0);
	mesh.set_vertex_attribute("velocity_in", vel_buf, 3, GL_FLOAT, 0, 0);
	mesh.set_vertex_attribute("lifetime_in", life_buf, 2, GL_FLOAT, 0, 0);
}

void Particles::draw() {
	if(!mesh.shader) {
		App::error_string.push_back("No shader set.");
	}
	mesh.shader->bind();
	mesh.shader->set_uniform_float("time", (float)timer.get_time_s());
	mesh.draw(GL_POINTS);
}
