#include "camera.h"

using namespace sgltk;

Camera::Camera(glm::vec3 pos,
	       glm::vec3 dir,
	       glm::vec3 up) {

	this->pos = pos;
	this->dir = glm::normalize(dir);
	this->up = glm::normalize(up);
	right = glm::normalize(glm::cross(dir, up));
	this->up = glm::normalize(glm::cross(right, dir));
	width = 640.f;
	height = 480.f;
	fovy = (float)M_PI;
	near_plane = 1.0f;
	far_plane = 800.f;

	update_view_matrix();
}

Camera::~Camera() {
}

void Camera::update_view_matrix() {
	view_matrix = glm::lookAt(pos, pos + dir, up);
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

void Camera::move_by(float x, float y, float z) {
	pos.x += x;
	pos.y += y;
	pos.z += z;
}

void Camera::move_by(glm::vec3 vector) {
	pos += vector;
}

void Camera::yaw(float angle) {
	glm::mat3 rot = glm::mat3(glm::rotate(angle, up));
	dir = glm::normalize(rot * dir);
	right = glm::normalize(rot * right);
}

void Camera::roll(float angle) {
	glm::mat3 rot = glm::mat3(glm::rotate(angle, dir));
	up = glm::normalize(rot * up);
	right = glm::normalize(rot * right);
}

void Camera::pitch(float angle) {
	glm::mat3 rot = glm::mat3(glm::rotate(angle, right));
	dir = glm::normalize(rot * dir);
	up = glm::normalize(rot * up);
}

