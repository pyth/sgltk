#include "camera.h"

using namespace sgltk;

P_Camera::P_Camera() : Camera(glm::vec3(0, 0, 0),
	glm::vec3(0, 0, -1),
	glm::vec3(0, 1, 0)) {

	update_projection_matrix(fovy, width, height, near_plane, far_plane);
}

P_Camera::P_Camera(glm::vec3 pos,
	glm::vec3 dir,
	glm::vec3 up) :
	Camera(pos, dir, up) {

	update_projection_matrix(fovy, width, height, near_plane, far_plane);
};

P_Camera::P_Camera(glm::vec3 pos,
	glm::vec3 dir,
	glm::vec3 up,
	float fovy,
	float width,
	float height,
	float near_plane,
	float far_plane) :
	Camera(pos, dir, up) {

	this->fovy = fovy;
	this->width = width;
	this->height = height;
	this->near_plane = near_plane;
	this->far_plane = far_plane;
	update_projection_matrix(fovy, width, height, near_plane, far_plane);
}

P_Camera::~P_Camera() {
}

void P_Camera::update_projection_matrix(float width,
	float height) {
	this->width = width;
	this->height = height;

	update_projection_matrix(fovy, width, height, near_plane, far_plane);
}

void P_Camera::update_projection_matrix(float fovy,
	float width,
	float height) {

	this->fovy = fovy;
	this->width = width;
	this->height = height;

	update_projection_matrix(fovy, width, height, near_plane, far_plane);
}

void P_Camera::update_projection_matrix(float fovy,
	float width,
	float height,
	float near_plane,
	float far_plane) {

	this->fovy = fovy;
	this->width = width;
	this->height = height;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

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

	float near_y = tan(fovy) * near_plane;
	float near_x = width / height * near_y;

	float far_y = tan(fovy) * far_plane;
	float far_x = width / height * far_y;

	glm::vec3 cam_dir = glm::normalize(dir);
	glm::vec3 cam_right = glm::normalize(right);
	glm::vec3 cam_up = glm::normalize(up);

	glm::vec3 near_center = pos + near_plane * cam_dir;
	glm::vec3 far_center = pos + far_plane * cam_dir;

	if(near_bottom_left)
		*near_bottom_left = near_center
			- near_x * cam_right
			- near_y * cam_up;
	if(near_bottom_right)
		*near_bottom_right = near_center
			+ near_x * cam_right
			- near_y * cam_up;
	if(near_top_right)
		*near_top_right = near_center
			+ near_x * cam_right
			+ near_y * cam_up;
	if(near_top_left)
		*near_top_left = near_center
			- near_x * cam_right
			+ near_y * cam_up;
	if(far_bottom_left)
		*far_bottom_left = far_center
			- far_x * cam_right
			- far_y * cam_up;
	if(far_bottom_right)
		*far_bottom_right = far_center
			+ far_x * cam_right
			- far_y * cam_up;
	if(far_top_right)
		*far_top_right = far_center
			+ far_x * cam_right
			+ far_y * cam_up;
	if(far_top_left)
		*far_top_left = far_center
			- far_x * cam_right
			+ far_y * cam_up;
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

	glm::vec3 cam_dir = glm::normalize(dir);

	glm::vec3 far_center = pos + far_plane * cam_dir;
	glm::vec3 near_center = pos + near_plane * cam_dir;

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
