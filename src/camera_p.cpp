#include "camera.h"

using namespace sgltk;

P_Camera::P_Camera() : Camera(glm::vec3(0, 0, 0),
			      glm::vec3(0, 0, -1),
			      glm::vec3(0, 1, 0)) {

	update_projection_matrix();
}

P_Camera::P_Camera(const P_Camera& camera) :
		Camera(camera.position, camera.direction, camera.up) {

	fovy = camera.fovy;
	width = camera.width;
	height = camera.height;
	near_plane = camera.near_plane;
	far_plane = camera.far_plane;
	update_projection_matrix();
}

P_Camera::P_Camera(glm::vec3 position,
		   glm::vec3 direction,
		   glm::vec3 up) :
		   Camera(position, direction, up) {

	update_projection_matrix();
};

P_Camera::P_Camera(glm::vec3 position,
		   glm::vec3 direction,
		   glm::vec3 up,
		   float fovy,
		   float width,
		   float height,
		   float near_plane,
		   float far_plane) :
		   Camera(position, direction, up) {

	this->fovy = fovy;
	this->width = width;
	this->height = height;
	this->near_plane = near_plane;
	this->far_plane = far_plane;
	update_projection_matrix();
}

P_Camera::~P_Camera() {
}

void P_Camera::update_projection_matrix() {

	projection_matrix = glm::perspective(fovy,
					     (GLfloat)width / (GLfloat)height,
					     near_plane, far_plane);
}

std::vector<glm::vec3> P_Camera::calculate_frustum_points() {

	std::vector<glm::vec4> ndc = {
		glm::vec4(-1, -1, -1,  1),
		glm::vec4( 1, -1, -1,  1),
		glm::vec4( 1,  1, -1,  1),
		glm::vec4(-1,  1, -1,  1),
		glm::vec4(-1, -1,  1,  1),
		glm::vec4( 1, -1,  1,  1),
		glm::vec4( 1,  1,  1,  1),
		glm::vec4(-1,  1,  1,  1)
	};

	glm::mat4 mat = glm::inverse(projection_matrix * view_matrix);

	glm::vec4 tmp;
	std::vector<glm::vec3> ret(ndc.size());
	for(unsigned int i = 0; i < ndc.size(); i++) {
		tmp = mat * ndc[i];
		ret[i] = glm::vec3(tmp) / tmp[3];
	}
	return ret;
}

std::vector<float> P_Camera::calculate_frustum_distance(glm::vec3 point) {

	glm::vec3 cam_dir = glm::normalize(direction);

	glm::vec3 far_center = point + far_plane * cam_dir;
	glm::vec3 near_center = point + near_plane * cam_dir;

	std::vector<glm::vec3> fpoints = calculate_frustum_points();

	glm::vec3 left_normal = glm::normalize(glm::cross(fpoints[7] - fpoints[3],
							  fpoints[0] - fpoints[3]));
	glm::vec3 right_normal = glm::normalize(glm::cross(fpoints[1] - fpoints[2],
							   fpoints[6] - fpoints[2]));
	glm::vec3 top_normal = glm::normalize(glm::cross(fpoints[2] - fpoints[3],
							 fpoints[7] - fpoints[3]));
	glm::vec3 bottom_normal = glm::normalize(glm::cross(fpoints[4] - fpoints[0],
							    fpoints[1] - fpoints[0]));

	std::vector<float> ret(6);

	ret[0] = glm::dot(cam_dir, position - far_center);
	ret[1] = glm::dot(-cam_dir, position - near_center);
	ret[2] = glm::dot(left_normal, position - fpoints[3]);
	ret[3] = glm::dot(right_normal, position - fpoints[2]);
	ret[4] = glm::dot(top_normal, position - fpoints[3]);
	ret[5] = glm::dot(bottom_normal, position - fpoints[0]);

	return ret;
}
