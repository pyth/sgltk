#include "camera.h"

Camera::Camera() {
	Camera(glm::vec3(0,0,1), glm::vec3(0,0,-1), glm::vec3(0,1,0),
	       70.0f, 640, 480, 1.0f, 800.0f);
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up) {
	Camera(pos, dir, up, 70.0f, 640, 480, 1.0f, 800.0f);
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float fov,
	       float width, float height, float near_plane, float far_plane) {
	ident = glm::mat4(1);

	this->pos = glm::vec4(pos, 1);
	this->dir = glm::vec4(dir, 1);
	this->up = glm::vec4(up, 1);
	right = glm::vec4(glm::cross(dir, up), 1);
	this->fov = fov;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	update_view_matrix();
	update_projection_matrix(fov, width, height, near_plane, far_plane);
}

Camera::~Camera() {
}

void Camera::update_view_matrix() {
	view_matrix = glm::lookAt(glm::vec3(pos),
				  glm::vec3(pos + dir),
				  glm::vec3(up));
}

void Camera::update_projection_matrix(float width, float height) {
	update_projection_matrix(fov, width, height, near_plane, far_plane);
}

void Camera::update_projection_matrix(float fov, float width, float height) {
	update_projection_matrix(fov, width, height, near_plane, far_plane);
}

void Camera::update_projection_matrix(float fov, float width, float height,
				      float near_plane, float far_plane) {
	projection_matrix = glm::perspective(fov,
				(GLfloat)width/(GLfloat)height,
				near_plane, far_plane);
}

void Camera::move_up(float delta) {
	pos += glm::normalize(up) * delta;
}

void Camera::move_right(float delta) {
	pos += glm::normalize(right) * delta;
}

void Camera::move_forward(float delta) {
	pos += glm::normalize(dir) * delta;
}

void Camera::yaw(float angle) {
	glm::mat4 rot = glm::rotate(ident, angle, glm::vec3(up));
	dir = rot * dir;
	right = rot * right;
}

void Camera::roll(float angle) {
	glm::mat4 rot = glm::rotate(ident, angle, glm::vec3(dir));
	up = rot * up;
	right = rot * right;
}

void Camera::pitch(float angle) {
	glm::mat4 rot = glm::rotate(ident, angle, glm::vec3(right));
	dir = rot * dir;
	up = rot * up;
}
