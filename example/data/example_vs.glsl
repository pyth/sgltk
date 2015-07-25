#version 130

struct Matrix {
	mat4 model_view;
	mat4 model_view_proj;
	mat3 normal_matrix;
};

in vec4 pos;
in vec3 norm;
in vec3 tc_in;

out vec4 pos_eye;
out vec3 norm_eye;
out vec3 tc;

uniform Matrix matrix;
uniform sampler2D Texture;

void main() {
	pos_eye = matrix.model_view * pos;
	norm_eye = matrix.normal_matrix * norm;

	tc = tc_in;

	gl_Position = matrix.model_view_proj * pos;
}
