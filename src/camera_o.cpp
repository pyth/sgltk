#include <sgltk/camera.h>

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

std::vector<glm::vec3> O_Camera::calculate_frustum_points() {

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

	std::vector<glm::vec3> ret(ndc.size());
	for(unsigned int i = 0; i < ndc.size(); i++) {
		ret[i] = glm::vec3(mat * ndc[i]);
	}
	return ret;
}

std::vector<float> O_Camera::calculate_frustum_distance(glm::vec3 point) {

	glm::vec3 cam_dir = glm::normalize(direction);

	glm::vec3 far_center = point + far_plane * cam_dir;
	glm::vec3 near_center = point + near_plane * cam_dir;

	std::vector<glm::vec3> fpoints = calculate_frustum_points();

	glm::vec3 left_normal = glm::cross(fpoints[7] - fpoints[3], fpoints[0] - fpoints[3]);
	glm::vec3 right_normal = glm::cross(fpoints[1] - fpoints[2], fpoints[6] - fpoints[2]);
	glm::vec3 top_normal = glm::cross(fpoints[7] - fpoints[3], fpoints[2] - fpoints[3]);
	glm::vec3 bottom_normal = glm::cross(fpoints[1] - fpoints[0], fpoints[4] - fpoints[0]);

	std::vector<float> ret(6);
	ret[0] = glm::dot(position - far_center, cam_dir);
	ret[1] = glm::dot(position - near_center, -cam_dir);
	ret[2] = glm::dot(position - fpoints[3], left_normal);
	ret[3] = glm::dot(position - fpoints[2], right_normal);
	ret[4] = glm::dot(position - fpoints[3], top_normal);
	ret[5] = glm::dot(position - fpoints[0], bottom_normal);

	return ret;
}

void O_Camera::calculate_bounding_frustum(O_Camera& camera, glm::vec3 direction, float offset) {

	std::vector<glm::vec3> fpoints = camera.calculate_frustum_points();

	glm::vec3 forward = glm::normalize(direction);
	glm::vec3 right = normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
	glm::vec3 up = normalize(glm::cross(right, forward));

	glm::mat4 lm = glm::lookAt(glm::vec3(0), forward, up);
	glm::mat4 lm_inv = glm::inverse(lm);

	for(unsigned int i = 0; i < fpoints.size(); i++) {
		fpoints[i] = glm::vec3(lm * glm::vec4(fpoints[i], 1));
	}

	glm::vec3 min = fpoints[0];
	glm::vec3 max = fpoints[0];

	for(int i = 1; i < 8; i++) {
		for(int j = 0; j < 3; j++) {
			if(fpoints[i][j] > max[j])
				max[j] = fpoints[i][j];
			else if(fpoints[i][j] < min[j])
				min[j] = fpoints[i][j];
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

	std::vector<glm::vec3> fpoints = camera.calculate_frustum_points();

	glm::vec3 forward = glm::normalize(direction);
	glm::vec3 right = normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
	glm::vec3 up = normalize(glm::cross(right, forward));

	glm::mat4 lm = glm::lookAt(glm::vec3(0), forward, up);
	glm::mat4 lm_inv = glm::inverse(lm);

	for(unsigned int i = 0; i < fpoints.size(); i++) {
		fpoints[i] = glm::vec3(lm * glm::vec4(fpoints[i], 1));
	}

	glm::vec3 min = fpoints[0];
	glm::vec3 max = fpoints[0];

	for(int i = 1; i < 8; i++) {
		for(int j = 0; j < 3; j++) {
			if(fpoints[i][j] > max[j])
				max[j] = fpoints[i][j];
			else if(fpoints[i][j] < min[j])
				min[j] = fpoints[i][j];
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
