#include "camera.h"

using namespace sgltk;

IP_Camera::IP_Camera() : Camera(glm::vec3(0, 0, 0),
	glm::vec3(0, 0, -1),
	glm::vec3(0, 1, 0)) {

	update_projection_matrix(fovy, width, height, near_plane);
}

IP_Camera::IP_Camera(glm::vec3 pos,
	glm::vec3 dir,
	glm::vec3 up) :
	Camera(pos, dir, up) {

	update_projection_matrix(fovy, width, height, near_plane);
};

IP_Camera::IP_Camera(glm::vec3 pos,
	glm::vec3 dir,
	glm::vec3 up,
	float fovy,
	float width,
	float height,
	float near_plane) :
	Camera(pos, dir, up) {

	this->fovy = fovy;
	this->width = width;
	this->height = height;
	this->near_plane = near_plane;
	update_projection_matrix(fovy, width, height, near_plane);
}

IP_Camera::~IP_Camera() {
}

void IP_Camera::update_projection_matrix(float width,
	float height) {

	this->width = width;
	this->height = height;

	update_projection_matrix(fovy, width, height, near_plane);
}

void IP_Camera::update_projection_matrix(float fovy,
	float width,
	float height) {

	this->fovy = fovy;
	this->width = width;
	this->height = height;

	update_projection_matrix(fovy, width, height, near_plane);
}

void IP_Camera::update_projection_matrix(float fovy,
	float width,
	float height,
	float near_plane) {

	this->fovy = fovy;
	this->width = width;
	this->height = height;
	this->near_plane = near_plane;

	projection_matrix = glm::infinitePerspective(fovy,
		(GLfloat)width / (GLfloat)height,
		near_plane);
}