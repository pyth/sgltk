#include "camera.h"

using namespace sgltk;

Camera::Camera(glm::vec3 position,
	       glm::vec3 direction,
	       glm::vec3 up) {

	this->position = position;
	this->direction = glm::normalize(direction);
	right = glm::normalize(glm::cross(direction, glm::normalize(up)));
	this->up = glm::normalize(glm::cross(right, direction));
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
	view_matrix = glm::lookAt(position, position + direction, up);
}

void Camera::move_up(float delta) {
	position += glm::normalize(up) * delta;
}

void Camera::move_right(float delta) {
	position += glm::normalize(right) * delta;
}

void Camera::move_forward(float delta) {
	position += glm::normalize(direction) * delta;
}

void Camera::move_by(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
}

void Camera::move_by(glm::vec3 vector) {
	position += vector;
}

void Camera::yaw(float angle) {
	glm::mat3 rot = glm::mat3(glm::rotate(angle, up));
	direction = glm::normalize(rot * direction);
	right = glm::normalize(rot * right);
}

void Camera::roll(float angle) {
	glm::mat3 rot = glm::mat3(glm::rotate(angle, direction));
	up = glm::normalize(rot * up);
	right = glm::normalize(rot * right);
}

void Camera::pitch(float angle) {
	glm::mat3 rot = glm::mat3(glm::rotate(angle, right));
	direction = glm::normalize(rot * direction);
	up = glm::normalize(rot * up);
}
