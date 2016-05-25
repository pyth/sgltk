#include "camera.h"

using namespace sgltk;

Camera::Camera() {
	sgltk::init_lib();
	type = sgltk::PERSPECTIVE;

	pos = glm::vec4(0, 0, 1, 1);
	dir = glm::normalize(glm::vec4(0, 0, -1, 1));
	up = glm::normalize(glm::vec4(0, 1, 0, 1));
	right = glm::normalize(glm::vec4(glm::cross(glm::vec3(dir), glm::vec3(up)), 1));
	fov = 70.0f;
	near_plane = 1.0f;
	far_plane = 800.0f;

	update_view_matrix();
	update_projection_matrix(fov, 640, 480, near_plane, far_plane);
}

Camera::Camera(sgltk::CAMERA_TYPE type) {
	sgltk::init_lib();
	this->type = type;

	pos = glm::vec4(0, 0, 1, 1);
	dir = glm::normalize(glm::vec4(0, 0, -1, 1));
	up = glm::normalize(glm::vec4(0, 1, 0, 1));
	right = glm::normalize(glm::vec4(glm::cross(glm::vec3(dir), glm::vec3(up)), 1));
	fov = 70.0f;
	near_plane = 1.0f;
	far_plane = 800.0f;

	update_view_matrix();
	update_projection_matrix(fov, 640, 480, near_plane, far_plane);
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up) {
	sgltk::init_lib();
	type = sgltk::PERSPECTIVE;

	this->pos = glm::vec4(pos, 1);
	this->dir = glm::normalize(glm::vec4(dir, 1));
	this->up = glm::normalize(glm::vec4(up, 1));
	right = glm::normalize(glm::vec4(glm::cross(dir, up), 1));
	fov = 70.0f;
	near_plane = 1.0f;
	far_plane = 800.0f;

	update_view_matrix();
	update_projection_matrix(fov, 640, 480, near_plane, far_plane);
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up,
	       sgltk::CAMERA_TYPE type) {
	sgltk::init_lib();
	this->type = type;

	this->pos = glm::vec4(pos, 1);
	this->dir = glm::normalize(glm::vec4(dir, 1));
	this->up = glm::normalize(glm::vec4(up, 1));
	right = glm::normalize(glm::vec4(glm::cross(dir, up), 1));
	fov = 70.0f;
	near_plane = 1.0f;
	far_plane = 800.0f;

	update_view_matrix();
	update_projection_matrix(fov, 640, 480, near_plane, far_plane);
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float fov,
	       float width, float height, float near_plane, float far_plane) {
	sgltk::init_lib();
	type = sgltk::PERSPECTIVE;

	this->pos = glm::vec4(pos, 1);
	this->dir = glm::normalize(glm::vec4(dir, 1));
	this->up = glm::normalize(glm::vec4(up, 1));
	right = glm::normalize(glm::vec4(glm::cross(dir, up), 1));
	this->fov = fov;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	update_view_matrix();
	update_projection_matrix(fov, width, height, near_plane, far_plane);
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, float fov,
	       float width, float height, float near_plane, float far_plane, sgltk::CAMERA_TYPE type) {
	sgltk::init_lib();
	this->type = type;

	this->pos = glm::vec4(pos, 1);
	this->dir = glm::normalize(glm::vec4(dir, 1));
	this->up = glm::normalize(glm::vec4(up, 1));
	right = glm::normalize(glm::vec4(glm::cross(dir, up), 1));
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
	this->width = width;
	this->height = height;

	update_projection_matrix(fov, width, height, near_plane, far_plane);
}

void Camera::update_projection_matrix(float fov, float width, float height) {
	this->fov = fov;
	this->width = width;
	this->height = height;


	update_projection_matrix(fov, width, height, near_plane, far_plane);
}

void Camera::update_projection_matrix(float fov, float width, float height,
				      float near_plane) {
	this->fov = fov;
	this->width = width;
	this->height = height;
	this->near_plane = near_plane;

	update_projection_matrix(fov, width, height, near_plane, far_plane);
}

void Camera::update_projection_matrix(float fov, float width, float height,
				      float near_plane, float far_plane) {
	this->fov = fov;
	this->width = width;
	this->height = height;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	if(type & sgltk::PERSPECTIVE) {
		projection_matrix_persp = glm::perspective(fov,
					  (GLfloat)width/(GLfloat)height,
					  near_plane, far_plane);
	}
	if(type & sgltk::INF_PERSPECTIVE) {
		projection_matrix_persp_inf = glm::infinitePerspective(fov,
					      (GLfloat)width/(GLfloat)height,
					      near_plane);
	}
	if(type & sgltk::ORTHOGRAPHIC) {
		projection_matrix_ortho = glm::ortho(0.0f, width, 0.0f, height,
						     near_plane, far_plane);
	}
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
	glm::mat4 rot = glm::rotate(angle, glm::vec3(up));
	dir = glm::normalize(rot * dir);
	right = glm::normalize(rot * right);
}

void Camera::roll(float angle) {
	glm::mat4 rot = glm::rotate(angle, glm::vec3(dir));
	up = glm::normalize(rot * up);
	right = glm::normalize(rot * right);
}

void Camera::pitch(float angle) {
	glm::mat4 rot = glm::rotate(angle, glm::vec3(right));
	dir = glm::normalize(rot * dir);
	up = glm::normalize(rot * up);
}
