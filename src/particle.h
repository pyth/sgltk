#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "app.h"
#include "mesh.h"
#include "timer.h"
#include "shader.h"
#include "camera.h"

namespace sgltk {

/**
 * @class Particles
 * @brief The implementation of a particle system using the mesh class
 */
class Particles {
	Mesh mesh;
	Timer timer;
	unsigned int pos_buf;
	unsigned int vel_buf;
	unsigned int life_buf;
	unsigned int number;

	std::vector<glm::vec3> position;
	std::vector<glm::vec3> velocity;
	std::vector<glm::vec2> lifetime;
	std::vector<unsigned int> indices;
public:
	EXPORT Particles();
	EXPORT ~Particles();

	/**
	 * @brief Sets the number of the particles in the system
	 * @param number The new number of particles
	 */
	EXPORT void resize(unsigned int number);
	/**
	 * @brief Specifies the shader to use to render the particles
	 * @param shader The shader to be used to render the particles
	 */
	EXPORT void setup_shader(sgltk::Shader *shader);
	/**
	 * @brief Sets up the view and projection matrices that will be
	 *	  used to render the particles
	 * @param camera The camera to be used to draw the particles
	 */
	EXPORT void setup_camera(sgltk::Camera *camera);
	/**
	 * @brief Sets up the view and projection matrices that will be
	 *	  used to render the particles
	 * @param view_matrix The view matrix
	 * @param projection_matrix The projection matrix
	 */
	EXPORT void setup_camera(glm::mat4 *view_matrix,
			  glm::mat4 *projection_matrix);
	/**
	 * @brief Attaches a texture to the particles
	 */
	EXPORT void attach_texture(const std::string& name,
			    sgltk::Texture *texture);
	/**
	 * @brief Adds a new particle to the system if the particle buffer has an empty space
	 * @param position The position of the new particle
	 * @param velocity The velocity vector of the new particle
	 * @param lifetime The lifetime of the particle in seconds 
	 * @return Returns true if the particle was successfully added to the buffer, 
	 * 	false otherwise
	 */
	EXPORT bool add_particle(glm::vec3 position,
			  glm::vec3 velocity,
			  float lifetime);
	/**
	 * @brief Updates the particle buffers
	 */
	EXPORT void update();
	/**
	 * @brief Draws the particles
	 */
	EXPORT void draw();
};

}

#endif
