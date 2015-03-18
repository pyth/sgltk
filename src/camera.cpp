#include "camera.h"

Camera::Camera() {
	ident = glm::mat4(1);

	up = glm::vec4(0,1,0,1);
	pos = glm::vec4(0);
	right = glm::vec4(1,0,0,1);
	forward = glm::vec4(0,0,1,1);
}

Camera::~Camera() {
}

void Camera::move_up(float delta) {
	pos += glm::normalize(up) * delta;
}

void Camera::move_right(float delta) {
	pos += glm::normalize(right) * delta;
}

void Camera::move_forward(float delta) {
	pos += glm::normalize(forward) * delta;
}

void Camera::yaw(float angle) {
	glm::mat4 rot = glm::rotate(ident, angle, glm::vec3(up));
	forward = rot * forward;
	right = rot * right;
}

void Camera::roll(float angle) {
	glm::mat4 rot = glm::rotate(ident, angle, glm::vec3(forward));
	up = rot * up;
	right = rot * right;
}

void Camera::pitch(float angle) {
	glm::mat4 rot = glm::rotate(ident, angle, glm::vec3(right));
	forward = rot * forward;
	up = rot * up;
}