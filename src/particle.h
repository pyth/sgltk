#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "core.h"

namespace sgltk {

class Particles {
	int number;
	glm::vec3 origin;
	glm::vec3 velocity;
	glm::vec3 origin_offset;
	glm::vec3 velocity_offset;

public:
	Particles(int number, glm::vec3 origin, glm::vec3 velocity);
	Particles(int number, glm::vec3 origin, glm::vec3 velocity, glm::vec3 origin_offset, glm::vec3 velocity_offset);
	~Particles();

	void render();
};
}

#endif
