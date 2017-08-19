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

void P_Camera::calculate_frustum_points(glm::vec3 *near_bottom_left,
					glm::vec3 *near_bottom_right,
					glm::vec3 *near_top_right,
					glm::vec3 *near_top_left,
					glm::vec3 *far_bottom_left,
					glm::vec3 *far_bottom_right,
					glm::vec3 *far_top_right,
					glm::vec3 *far_top_left) {

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
	tmp = mat * ndc[0];
	*near_bottom_left = glm::vec3(tmp) / tmp[3];
	tmp = mat * ndc[1];
	*near_bottom_right = glm::vec3(tmp) / tmp[3];
	tmp = mat * ndc[2];
	*near_top_right = glm::vec3(tmp) / tmp[3];
	tmp = mat * ndc[3];
	*near_top_left = glm::vec3(tmp) / tmp[3];
	tmp = mat * ndc[4];
	*far_bottom_left = glm::vec3(tmp) / tmp[3];
	tmp = mat * ndc[5];
	*far_bottom_right = glm::vec3(tmp) / tmp[3];
	tmp = mat * ndc[6];
	*far_top_right = glm::vec3(tmp) / tmp[3];
	tmp = mat * ndc[7];
	*far_top_left = glm::vec3(tmp) / tmp[3];
}

void P_Camera::calculate_frustum_distance(glm::vec3 position,
						float *far,
						float *near,
						float *left,
						float *right,
						float *top,
						float *bottom) {

	glm::vec3 near_bottom_left;
	glm::vec3 near_bottom_right;
	glm::vec3 near_top_right;
	glm::vec3 near_top_left;
	glm::vec3 far_bottom_left;
	glm::vec3 far_bottom_right;
	glm::vec3 far_top_right;
	glm::vec3 far_top_left;

	glm::vec3 cam_dir = glm::normalize(direction);

	glm::vec3 far_center = position + far_plane * cam_dir;
	glm::vec3 near_center = position + near_plane * cam_dir;

	calculate_frustum_points(&near_bottom_left,
					&near_bottom_right,
					&near_top_right,
					&near_top_left,
					&far_bottom_left,
					&far_bottom_right,
					&far_top_right,
					&far_top_left);

	glm::vec3 left_normal = glm::normalize(glm::cross(far_top_left - near_top_left,
							near_bottom_left - near_top_left));
	glm::vec3 right_normal = glm::normalize(glm::cross(near_bottom_right - near_top_right,
							far_top_right - near_top_right));
	glm::vec3 top_normal = glm::normalize(glm::cross(near_top_right - near_top_left,
							far_top_left - near_top_left));
	glm::vec3 bottom_normal = glm::normalize(glm::cross(far_bottom_left - near_bottom_left,
							near_bottom_right - near_bottom_left));

	if(far) {
		*far = glm::dot(cam_dir, position - far_center);
	}
	if(near) {
		*near = glm::dot(-cam_dir, position - near_center);
	}
	if(left) {
		*left = glm::dot(left_normal, position - near_top_left);
	}
	if(right) {
		*right = glm::dot(right_normal, position - near_top_right);
	}
	if(top) {
		*top = glm::dot(top_normal, position - near_top_left);
	}
	if(bottom) {
		*bottom = glm::dot(bottom_normal, position - near_bottom_left);
	}
}
