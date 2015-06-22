#include "camera.h"

Camera::Camera() {
	Camera(glm::vec3(0,0,1), glm::vec3(0,0,-1), glm::vec3(0,1,0));
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up) {
	ident = glm::mat4(1);

	this->pos = glm::vec4(pos, 1);
	this->dir = glm::vec4(dir, 1);
	this->up = glm::vec4(up, 1);
	right = glm::vec4(glm::cross(dir, up), 1);
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
