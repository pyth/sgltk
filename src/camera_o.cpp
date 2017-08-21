#include "camera.h"

using namespace sgltk;

O_Camera::O_Camera() : Camera(glm::vec3(0, 0, 0),
			      glm::vec3(0, 0, -1),
			      glm::vec3(0, 1, 0)) {

	update_projection_matrix();
}

O_Camera::O_Camera(const O_Camera& camera) :
		Camera(camera.position, camera.direction, camera.up) {

	width = camera.width;
	height = camera.height;
	near_plane = camera.near_plane;
	far_plane = camera.far_plane;
	update_projection_matrix();
}

O_Camera::O_Camera(glm::vec3 position,
		   glm::vec3 direction,
		   glm::vec3 up) :
		   Camera(position, direction, up) {

	update_projection_matrix();
};

O_Camera::O_Camera(glm::vec3 position,
		   glm::vec3 direction,
		   glm::vec3 up,
		   float width,
		   float height,
		   float near_plane,
		   float far_plane) :
		   Camera(position, direction, up) {

	this->width = width;
	this->height = height;
	this->near_plane = near_plane;
	this->far_plane = far_plane;
	update_projection_matrix();
}

O_Camera::~O_Camera() {
}

void O_Camera::update_projection_matrix() {

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

	std::vector<glm::vec4> ndc = {
		glm::vec4(-1, -1, -1,  1),
		glm::vec4( 1, -1, -1,  1),
		glm::vec4( 1,  1, -1,  1),
		glm::vec4(-1,  1, -1,  1),
		glm::vec4(-1, -1,  1,  1),
		glm::vec4( 1, -1,  1,  1),
		glm::vec4( 1,  1,  1,  1),
		glm::vec4(-1,  1,  1,  1)
	};

	glm::mat4 mat = glm::inverse(projection_matrix * view_matrix);

	*near_bottom_left = glm::vec3(mat * ndc[0]);
	*near_bottom_right = glm::vec3(mat * ndc[1]);
	*near_top_right = glm::vec3(mat * ndc[2]);
	*near_top_left = glm::vec3(mat * ndc[3]);
	*far_bottom_left = glm::vec3(mat * ndc[4]);
	*far_bottom_right = glm::vec3(mat * ndc[5]);
	*far_top_right = glm::vec3(mat * ndc[6]);
	*far_top_left = glm::vec3(mat * ndc[7]);
}

void O_Camera::calculate_frustum_distance(glm::vec3 position,
					  float *far, float *near,
					  float *left, float *right,
					  float *top, float *bottom) {

	glm::vec3 near_bottom_left;
	glm::vec3 near_bottom_right;
	glm::vec3 near_top_right;
	glm::vec3 near_top_left;
	glm::vec3 far_bottom_left;
	glm::vec3 far_bottom_right;
	glm::vec3 far_top_right;
	glm::vec3 far_top_left;

	glm::vec3 cam_dir = glm::normalize(direction);

	glm::vec3 far_center = position + far_plane * cam_dir;
	glm::vec3 near_center = position + near_plane * cam_dir;

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

void O_Camera::calculate_bounding_frustum(O_Camera& camera, glm::vec3 direction, float offset) {
	std::vector<glm::vec3> frustum_points(8);

	camera.calculate_frustum_points(&frustum_points[0], &frustum_points[1],
					&frustum_points[2], &frustum_points[3],
					&frustum_points[4], &frustum_points[5],
					&frustum_points[6], &frustum_points[7]);

	glm::vec3 forward = glm::normalize(direction);
	glm::vec3 right = normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
	glm::vec3 up = normalize(glm::cross(right, forward));

	glm::mat4 lm = glm::lookAt(glm::vec3(0), forward, up);
	glm::mat4 lm_inv = glm::inverse(lm);

	for(unsigned int i = 0; i < frustum_points.size(); i++) {
		frustum_points[i] = glm::vec3(lm * glm::vec4(frustum_points[i], 1));
	}

	glm::vec3 min = frustum_points[0];
	glm::vec3 max = frustum_points[0];

	for(int i = 1; i < 8; i++) {
		for(int j = 0; j < 3; j++) {
			if(frustum_points[i][j] > max[j])
				max[j] = frustum_points[i][j];
			else if(frustum_points[i][j] < min[j])
				min[j] = frustum_points[i][j];
		}
	}

	far_plane = abs(max[2] - min[2]);
	width = abs(max[0] - min[0]);
	height = abs(max[1] - min[1]);

	position = glm::vec3(lm_inv * glm::vec4(0.5f * (min + max), 1));

	update_view_matrix();
	projection_matrix = glm::ortho(-0.5f * width - offset, 0.5f * width + offset,
				       -0.5f * height - offset, 0.5f * height + offset,
				       -0.5f * far_plane - offset, 0.5f * far_plane + offset);
}

void O_Camera::calculate_bounding_frustum(P_Camera& camera, glm::vec3 direction, float offset) {
	std::vector<glm::vec3> frustum_points(8);

	camera.calculate_frustum_points(&frustum_points[0], &frustum_points[1],
					&frustum_points[2], &frustum_points[3],
					&frustum_points[4], &frustum_points[5],
					&frustum_points[6], &frustum_points[7]);

	glm::vec3 forward = glm::normalize(direction);
	glm::vec3 right = normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
	glm::vec3 up = normalize(glm::cross(right, forward));

	glm::mat4 lm = glm::lookAt(glm::vec3(0), forward, up);
	glm::mat4 lm_inv = glm::inverse(lm);

	for(unsigned int i = 0; i < frustum_points.size(); i++) {
		frustum_points[i] = glm::vec3(lm * glm::vec4(frustum_points[i], 1));
	}

	glm::vec3 min = frustum_points[0];
	glm::vec3 max = frustum_points[0];

	for(int i = 1; i < 8; i++) {
		for(int j = 0; j < 3; j++) {
			if(frustum_points[i][j] > max[j])
				max[j] = frustum_points[i][j];
			else if(frustum_points[i][j] < min[j])
				min[j] = frustum_points[i][j];
		}
	}

	far_plane = abs(max[2] - min[2]);
	width = abs(max[0] - min[0]);
	height = abs(max[1] - min[1]);

	position = glm::vec3(lm_inv * glm::vec4(0.5f * (min + max), 1));

	update_view_matrix();
	projection_matrix = glm::ortho(-0.5f * width - offset, 0.5f * width + offset,
				       -0.5f * height - offset, 0.5f * height + offset,
				       -0.5f * far_plane - offset, 0.5f * far_plane + offset);
}
