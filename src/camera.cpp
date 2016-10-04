#include "camera.h"

using namespace sgltk;

Camera::Camera() {
	type = sgltk::PERSPECTIVE;

	pos = glm::vec4(0, 0, 1, 1);
	dir = glm::normalize(glm::vec4(0, 0, -1, 1));
	up = glm::normalize(glm::vec4(0, 1, 0, 1));
	right = glm::normalize(glm::vec4(glm::cross(glm::vec3(dir), glm::vec3(up)), 1));
	fovy = 70.0f;
	near_plane = 1.0f;
	far_plane = 800.0f;

	update_view_matrix();
	update_projection_matrix(fovy, 640, 480, near_plane, far_plane);
}

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up,
	       sgltk::CAMERA_TYPE type) {
	this->type = type;

	this->pos = glm::vec4(pos, 1);
	this->dir = glm::normalize(glm::vec4(dir, 1));
	this->up = glm::normalize(glm::vec4(up, 1));
	right = glm::normalize(glm::vec4(glm::cross(dir, up), 1));
	fovy = 70.0f;
	near_plane = 1.0f;
	far_plane = 800.0f;

	update_view_matrix();
	update_projection_matrix(fovy, 640, 480, near_plane, far_plane);
}

Camera::Camera(glm::vec3 pos,
	       glm::vec3 dir,
	       glm::vec3 up,
	       float fovy,
	       float width, float height,
	       float near_plane, float far_plane,
	       sgltk::CAMERA_TYPE type) {

	this->type = type;

	this->pos = glm::vec4(pos, 1);
	this->dir = glm::normalize(glm::vec4(dir, 1));
	this->up = glm::normalize(glm::vec4(up, 1));
	right = glm::normalize(glm::vec4(glm::cross(dir, up), 1));
	this->fovy = fovy;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	update_view_matrix();
	update_projection_matrix(fovy, width, height, near_plane, far_plane);
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

	update_projection_matrix(fovy, width, height, near_plane, far_plane);
}

void Camera::update_projection_matrix(float fovy, float width, float height) {
	this->fovy = fovy;
	this->width = width;
	this->height = height;


	update_projection_matrix(fovy, width, height, near_plane, far_plane);
}

void Camera::update_projection_matrix(float fovy, float width, float height,
				      float near_plane) {
	this->fovy = fovy;
	this->width = width;
	this->height = height;
	this->near_plane = near_plane;

	update_projection_matrix(fovy, width, height, near_plane, far_plane);
}

void Camera::update_projection_matrix(float fovy, float width, float height,
				      float near_plane, float far_plane) {
	this->fovy = fovy;
	this->width = width;
	this->height = height;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	if(type & sgltk::PERSPECTIVE) {
		projection_matrix_persp = glm::perspective(fovy,
					  (GLfloat)width/(GLfloat)height,
					  near_plane, far_plane);
	}
	if(type & sgltk::INF_PERSPECTIVE) {
		projection_matrix_persp_inf = glm::infinitePerspective(fovy,
					      (GLfloat)width/(GLfloat)height,
					      near_plane);
	}
	if(type & sgltk::ORTHOGRAPHIC) {
		projection_matrix_ortho = glm::ortho(-width / 2, width / 2,
						     -height / 2, height / 2,
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

void Camera::move_by(float x, float y, float z) {
	pos.x += x;
	pos.y += y;
	pos.z += z;
}

void Camera::move_by(glm::vec3 vector) {
	pos += glm::vec4(vector, 1);
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

void Camera::calculate_frustum_distance(glm::vec3 position,
					float *far,
					float *near,
					float *left,
					float *right,
					float *top,
					float *bottom) {
	glm::vec3 cam_pos = glm::vec3(pos);
	glm::vec3 cam_dir = glm::vec3(glm::normalize(dir));
	glm::vec3 cam_right = glm::vec3(this->right);
	glm::vec3 cam_up = glm::vec3(up);

	glm::vec3 far_center = cam_pos + far_plane * cam_dir;
	glm::vec3 near_center = cam_pos + near_plane * cam_dir;

	float far_y = 0.5f * glm::tan(glm::radians(fovy)) * far_plane;
	float far_x = far_y * 0.5f * width / height;

	glm::vec3 near_top_left = near_center
				  - 0.5f * width * cam_right
				  + 0.5f * height * cam_up;
	glm::vec3 near_top_right = near_center
				  + 0.5f * width * cam_right
				  + 0.5f * height * cam_up;
	glm::vec3 near_bottom_left = near_center
				  - 0.5f * width * cam_right
				  - 0.5f * height * cam_up;
	glm::vec3 near_bottom_right = near_center
				  + 0.5f * width * cam_right
				  - 0.5f * height * cam_up;
	glm::vec3 far_top_left = far_center
				 - far_x * cam_right
				 + far_y * cam_up;
	glm::vec3 far_top_right = far_center
				 + far_x * cam_right
				 + far_y * cam_up;
	glm::vec3 far_bottom_left = far_center
				    - far_x * cam_right
				    - far_y * cam_up;
	glm::vec3 left_normal = glm::cross(far_top_left - near_top_left,
					   near_bottom_left - near_top_left);
	glm::vec3 right_normal = glm::cross(near_bottom_right - near_top_right,
					    far_top_right - near_top_right);
	glm::vec3 top_normal = glm::cross(far_top_left - near_top_left,
					  near_top_right - near_top_left);
	glm::vec3 bottom_normal = glm::cross(near_bottom_right - near_bottom_left,
					     far_bottom_left - near_bottom_left);

	if(far) {
		*far = glm::dot(position - far_center, cam_dir);
	}
	if(near) {
		*near = glm::dot(position - near_center, -cam_dir);
	}
	if(left) {
		*left = glm::dot(position - near_top_left, left_normal);
	}
	if(right) {
		*right = glm::dot(position - near_top_right, right_normal);
	}
	if(top) {
		*top = glm::dot(position - near_top_left, top_normal);
	}
	if(bottom) {
		*bottom = glm::dot(position - near_bottom_left, bottom_normal);
	}
}
