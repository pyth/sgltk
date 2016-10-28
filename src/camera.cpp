#include "camera.h"

using namespace sgltk;

Camera::Camera(glm::vec3 pos,
	       glm::vec3 dir,
	       glm::vec3 up) {

	this->pos = pos;
	this->dir = glm::normalize(dir);
	this->up = glm::normalize(up);
	right = glm::normalize(glm::cross(dir, up));
	near_plane = 1.0f;

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

P_Camera::P_Camera() : Camera(glm::vec3(0, 0, 0),
			      glm::vec3(0, 0, -1),
			      glm::vec3(0, 1, 0)){


	fovy = (float)M_PI;
	width = 640.f;
	height = 480.f;
	near_plane = 0.1f;
	far_plane = 800.f;
	update_projection_matrix(fovy, width, height, near_plane, far_plane);
}

P_Camera::P_Camera(glm::vec3 pos,
		   glm::vec3 dir,
		   glm::vec3 up) :
		   Camera(pos, dir, up) {

	fovy = (float)M_PI;
	width = 640.f;
	height = 480.f;
	near_plane = 0.1f;
	far_plane = 800.f;
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
				(GLfloat)width/(GLfloat)height,
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

	float far_y = 0.5f * glm::tan(fovy) * far_plane;
	float far_x = far_y * 0.5f * width / height;

	glm::vec3 cam_dir = glm::normalize(dir);
	glm::vec3 cam_right = glm::normalize(right);
	glm::vec3 cam_up = glm::normalize(up);

	glm::vec3 far_center = pos + far_plane * cam_dir;
	glm::vec3 near_center = pos + near_plane * cam_dir;

	if(near_bottom_left)
		*near_bottom_left = near_center
					- 0.5f * width * cam_right
					- 0.5f * height * cam_up;
	if(near_bottom_right)
		*near_bottom_right = near_center
					+ 0.5f * width * cam_right
					- 0.5f * height * cam_up;
	if(near_top_right)
		*near_top_right = near_center
					+ 0.5f * width * cam_right
					+ 0.5f * height * cam_up;
	if(near_top_left)
		*near_top_left = near_center
					- 0.5f * width * cam_right
					+ 0.5f * height * cam_up;
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

O_Camera::O_Camera() : Camera(glm::vec3(0, 0, 0),
			      glm::vec3(0, 0, -1),
			      glm::vec3(0, 1, 0)){


	width = 640.f;
	height = 480.f;
	near_plane = 0.1f;
	far_plane = 800.f;
	update_projection_matrix(width, height, near_plane, far_plane);
}

O_Camera::O_Camera(glm::vec3 pos,
		   glm::vec3 dir,
		   glm::vec3 up) :
		   Camera(pos, dir, up) {

	width = 640.f;
	height = 480.f;
	near_plane = 0.1f;
	far_plane = 800.f;
	update_projection_matrix(width, height, near_plane, far_plane);
};

O_Camera::O_Camera(glm::vec3 pos,
		   glm::vec3 dir,
		   glm::vec3 up,
		   float width,
		   float height,
		   float near_plane,
		   float far_plane) :
		   Camera(pos, dir, up) {

	this->width = width;
	this->height = height;
	this->near_plane = near_plane;
	this->far_plane = far_plane;
	update_projection_matrix(width, height, near_plane, far_plane);
}

O_Camera::~O_Camera() {
}

void O_Camera::update_projection_matrix(float width,
					float height) {

	this->width = width;
	this->height = height;

	update_projection_matrix(width, height, near_plane, far_plane);
}

void O_Camera::update_projection_matrix(float width,
					float height,
					float near_plane,
					float far_plane) {

	this->width = width;
	this->height = height;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	projection_matrix = glm::ortho(-width / 2, width / 2,
				       -height / 2, height / 2,
				       near_plane, far_plane);
}

void O_Camera::calculate_frustum_points(glm::vec3 *near_bottom_left,
				      glm::vec3 *near_bottom_right,
				      glm::vec3 *near_top_right,
				      glm::vec3 *near_top_left,
				      glm::vec3 *far_bottom_left,
				      glm::vec3 *far_bottom_right,
				      glm::vec3 *far_top_right,
				      glm::vec3 *far_top_left) {

	float far_x = width / 2;
	float far_y = height / 2;

	glm::vec3 cam_dir = glm::normalize(dir);
	glm::vec3 cam_right = glm::normalize(right);
	glm::vec3 cam_up = glm::normalize(up);

	glm::vec3 far_center = pos + far_plane * cam_dir;
	glm::vec3 near_center = pos + near_plane * cam_dir;

	if(near_bottom_left)
		*near_bottom_left = near_center
					- 0.5f * width * cam_right
					- 0.5f * height * cam_up;
	if(near_bottom_right)
		*near_bottom_right = near_center
					+ 0.5f * width * cam_right
					- 0.5f * height * cam_up;
	if(near_top_right)
		*near_top_right = near_center
					+ 0.5f * width * cam_right
					+ 0.5f * height * cam_up;
	if(near_top_left)
		*near_top_left = near_center
					- 0.5f * width * cam_right
					+ 0.5f * height * cam_up;
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

void O_Camera::calculate_frustum_distance(glm::vec3 position,
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

IP_Camera::IP_Camera() : Camera(glm::vec3(0, 0, 0),
				glm::vec3(0, 0, -1),
				glm::vec3(0, 1, 0)){


	fovy = (float)M_PI;
	width = 640.f;
	height = 480.f;
	near_plane = 0.1f;
	update_projection_matrix(fovy, width, height, near_plane);
}

IP_Camera::IP_Camera(glm::vec3 pos,
		     glm::vec3 dir,
		     glm::vec3 up) :
		     Camera(pos, dir, up) {

	fovy = (float)M_PI;
	width = 640.f;
	height = 480.f;
	near_plane = 0.1f;
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
				(GLfloat)width/(GLfloat)height,
				near_plane);
}
