#include "particle.h"

using namespace sgltk;

Particles::Particles(int number, glm::vec3 origin, glm::vec3 velocity) {
	this->number = number;
	this->origin = origin;
	this->velocity = velocity;
	this->origin_offset = glm::vec3(0);
	this->velocity_offset = glm::vec3(0);
}

Particles::Particles(int number, glm::vec3 origin, glm::vec3 velocity, glm::vec3 origin_offset, glm::vec3 velocity_offset) {
	this->number = number;
	this->origin = origin;
	this->velocity = velocity;
	this->origin_offset = origin_offset;
	this->velocity_offset = velocity_offset;
}

Particles::~Particles() {
}
